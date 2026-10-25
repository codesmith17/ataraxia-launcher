# Engineering Guide: Remote NLP Categorization for Video Titles & Browsing History

This guide documents the best industry practices, architectures, and API integrations for sending video titles and browsing history to remote NLP APIs for real-time classification (e.g. **Educational vs. Entertainment / Distracting**).

---

## 🎯 Architecture Overview

```
┌────────────────────────────────────────────────────────┐
│                   ANDROID CLIENT                      │
│                                                        │
│   [Accessibility Event Capture]                        │
│     - YouTube Video Title + Channel Name               │
│     - Browser URL Bar + Tab Page Title                 │
│                 │                                      │
│                 ▼                                      │
│   [Local Level-1 In-Memory Cache (LRU)]                │
│     - Has this title/domain been classified already?   │
│       ├── YES ──► Return cached result (0ms, $0 cost)  │
│       └── NO  ──► Sanitize & Strip Personal Tokens     │
└─────────────────┬──────────────────────────────────────┘
                  │ HTTPS POST (TLS 1.3 / JSON)
                  ▼
┌────────────────────────────────────────────────────────┐
│               REMOTE NLP CLOUD ENGINE                  │
│                                                        │
│   Choice A: Ultra-Fast LLM API (Gemini / Groq / GPT-4o-mini)
│   Choice B: Zero-Shot HuggingFace Microservice (DeBERTa / BART)
│   Choice C: YouTube Data API v3 (Official Category IDs)
│   Choice D: Domain Taxonomy APIs (Klazify / Cloudflare Radar)
└─────────────────┬──────────────────────────────────────┘
                  │
                  ▼
         JSON Structured Verdict:
    {"category": "EDUCATIONAL", "confidence": 0.96, "action": "ALLOW"}
```

---

## 🏆 Top 4 Best Ways to Implement Remote Categorization

### 1. Ultra-Low Latency LLMs with Structured JSON (Best All-Rounder)
Using high-speed, cost-effective LLMs (such as **Gemini 1.5 Flash**, **Groq Llama-3.1-8B-Instant**, or **OpenAI GPT-4o-mini**):
- **Why it's the best:** Understands modern internet slang, sarcasm, clickbait, and nuance that static keyword filters fail on.
- **Latency:** ~120ms to 250ms (Groq or Gemini Flash).
- **Cost:** ~$0.05 to $0.15 per **1,000,000 tokens** (negligible).

#### Prompt Strategy (One-Shot JSON Output):
```http
POST https://api.groq.com/openai/v1/chat/completions
Authorization: Bearer $API_KEY
Content-Type: application/json

{
  "model": "llama-3.1-8b-instant",
  "temperature": 0.0,
  "response_format": { "type": "json_object" },
  "messages": [
    {
      "role": "system",
      "content": "You are a content classifier for a digital wellbeing app. Classify the user content into exactly one category: 'EDUCATIONAL', 'PRODUCTIVE', 'ENTERTAINMENT', or 'ADDICTIVE_CLICKBAIT'. Return strictly JSON with keys: category, confidence (0.0 to 1.0), and reason."
    },
    {
      "role": "user",
      "content": "Platform: YouTube | Channel: 3Blue1Brown | Title: The Essence of Calculus, Chapter 1"
    }
  ]
}
```

#### Expected JSON Response:
```json
{
  "category": "EDUCATIONAL",
  "confidence": 0.99,
  "reason": "Foundational mathematics tutorial on calculus."
}
```

---

### 2. The Native YouTube Data API v3 (Best for YouTube Accuracy)
If you capture the 11-character Video ID (e.g., `dQw4w9WgXcQ` from the accessibility link or URL):
- Call Google's official endpoint:
  ```http
  GET https://www.googleapis.com/youtube/v3/videos?part=snippet&id={VIDEO_ID}&key={API_KEY}
  ```
- Inspect the returned `categoryId`:
  | Category ID | Official YouTube Label | Classification |
  |:---:|---|:---:|
  | **27** | Education | ✅ **ALLOW** |
  | **28** | Science & Technology | ✅ **ALLOW** |
  | **26** | Howto & Style (Coding/Tutorials) | ✅ **ALLOW** |
  | **20** | Gaming | ❌ **BLOCK** |
  | **23** | Comedy | ❌ **BLOCK** |
  | **24** | Entertainment | ❌ **BLOCK** |
  | **10** | Music | ⚠️ Optional |

---

### 3. Serverless HuggingFace Zero-Shot Pipelines (Best for Self-Hosting)
If you want to run your own private cloud endpoint on **Cloudflare Workers AI**, **AWS Lambda**, or a small **HuggingFace Inference Endpoint**:
- Model: `facebook/bart-large-mnli` or `MoritzLaurer/DeBERTa-v3-base-mnli-fever-anli`.
- Send text with candidate labels: `["educational", "entertainment", "work", "gaming"]`.

```python
from transformers import pipeline

classifier = pipeline("zero-shot-classification", model="MoritzLaurer/DeBERTa-v3-base-mnli-fever-anli")

title = "I spent 100 days building a real operating system from scratch"
candidate_labels = ["Educational / Computer Science", "Entertainment / Gaming"]

output = classifier(title, candidate_labels)
# Output: 'Educational / Computer Science' (Score: 0.94)
```

---

### 4. Specialized URL & Browsing Categorization APIs
For classifying arbitrary website domains from the user's browser history:
1. **Cloudflare Radar API:**
   - Free API providing domain content categorization (e.g. `github.com` -> Technology, `reddit.com` -> Social, `netflix.com` -> Entertainment).
2. **Klazify API / Alpha Quantum API:**
   - Accepts any URL and returns the standard **IAB Content Taxonomy V2** (e.g. *Education > Computer Science* vs *Hobbies & Interests > Video Gaming*).

---

## 🔒 Critical Best Practices: Latency & Privacy Preservation

When transmitting titles and browsing history over the wire:

### 1. Two-Tier Caching (Crucial for Speed & Cost)
- Maintain a local **SQLite LRU Cache** on the phone.
- **Hit Ratio:** In testing, over **75%** of user browsing and video viewing hits repetitive domains and subscribed channels. 
- A local cache ensures sub-millisecond decisions for repeated visits with zero remote API hits.

### 2. Client-Side PII Scrubbing (Privacy Defense)
Never send raw tracking parameters or personal query tokens. Before transmitting:
- Strip query parameters: `?auth=`, `?token=`, `?session=`, `?user=`.
- Send only: `hostname + path_slug` and `clean_title`.

### 3. Asynchronous Debouncing
- When a user is typing a URL or scrolling rapidly through a video list, accessibility events fire 20 times per second.
- Debounce outgoing classification requests with a **400ms settle window** so API calls are only made when the user actually stops on a video.
