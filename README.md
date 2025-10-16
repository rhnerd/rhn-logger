# rhn-logger — Lightweight C Logger (ANSI colors + file)

A minimalist **single-header** logger for C.  
Provides **log levels**, **colored stderr output**, and optional **file logging** — with zero dependencies (besides the standard C headers: <stdio.h>, <time.h>, and <stdlib.h>).

---

## Features
- ✅ Header-only (`#include "log.h"`)
- ✅ Log levels: **DEBUG / INFO / WARN / ERROR / NONE**
- ✅ Colored logs to **stderr** (ANSI escape codes)
- ✅ Optional **file logging** (`LOG_FILE("app.log", "w")` or `"a"`)
- ✅ Compact format: `[HH:MM:SS] LEVEL message`
- ✅ No external dependencies

---

## Quick Start

1. Copy `log.h` into your project  
2. Include and log:

```c
#include "log.h"

int main(void) {
    LOG_INFO("stderr only, no file yet");

    LOG_FILE("app.log", "w");   // write - overwrite mode
    LOG_WARN("This goes to stderr + file (overwrite)");

    LOG_FILE_CLOSE();

    LOG_FILE("app.log", "a");   // append mode
    LOG_ERROR("This goes to stderr + file (append)");

    LOG_FILE_CLOSE();
    return 0;
}
```

---

## Log Levels & Build Configuration

Set default level at compile-time with `-DLOG_LEVEL=...`  
(e.g. `-DLOG_LEVEL=LOG_LEVEL_WARN`).

- `LOG_LEVEL_DEBUG` → debug + info + warn + error  
- `LOG_LEVEL_INFO` → info + warn + error  
- `LOG_LEVEL_WARN` → warn + error  
- `LOG_LEVEL_ERROR` → error only  
- `LOG_LEVEL_NONE` → disables all logging  

---

## API

```c
void LOG_FILE(const char *filename, const char *mode);
// enable logging to a file
// mode = "w" → overwrite (default)
// mode = "a" → append

void LOG_FILE_CLOSE(void); // close the file - file logging is optional

LOG_DEBUG(fmt, ...);
LOG_INFO(fmt, ...);
LOG_WARN(fmt, ...);
LOG_ERROR(fmt, ...);
```

---

## Example Output

```text
[12:00:01] INFO  Server started on port 8080
[12:00:02] DEBUG New connection from 127.0.0.1
[12:00:05] WARN  Connection took too long
[12:00:06] ERROR Failed to allocate buffer
```

(stderr will be **colorized** with ANSI escape codes, file logs are plain text)

---

## Philosophy (Why so simple?)

- Single-header, zero deps, drop-in.
- Works out of the box (stderr w/ colors).  
- File logging is optional (`LOG_FILE("app.log","w/a")`).  
- No hidden init frameworks, no threading overhead.

---

## Thread Safety

⚠️ `rhn-logger` is **not thread-safe by design**.  
If you need thread safety, wrap calls with a mutex in your app:

```c
pthread_mutex_lock(&mtx);
LOG_INFO("Hello from thread");
pthread_mutex_unlock(&mtx);
```

---

## FAQ

**Q:** What happens if I don’t call `LOG_FILE`?  
**A:** Logs are printed to `stderr` only (still colorized).  

**Q:** How do I append logs instead of overwriting?  
**A:** Use `LOG_FILE("app.log", "a")` instead of `"w"`.  

---

## License

MIT — do whatever you like, just keep attribution.  
