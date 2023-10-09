## Logger

The logging system allows you to print messages of different levels to facilitate tracking and debugging of your game.

### Log Levels

- **NoLog**: No messages are recorded.
- **Fatal**: Only "fatal" level messages are recorded.
- **Error**: "error" and "fatal" level messages are recorded.
- **Warn**: "warn", "error", and "fatal" level messages are recorded.
- **Info**: "info", "warn", "error", and "fatal" level messages are recorded.
- **Debug**: "debug", "info", "warn", "error", and "fatal" messages.
- **Trace**: "trace", "debug", "info", "warn", "error", and "fatal" messages.

### Usage

Watch out: Debug and Trace levels are only available in debug mode.

Header file: `#include "Logger.hpp"`

### Methods

Methods used to log messages of different levels.

```cpp
void Logger::fatal(const std::string &message);
void Logger::error(const std::string &message);
void Logger::warn(const std::string &message);
void Logger::info(const std::string &message);
void Logger::debug(const std::string &message);
void Logger::trace(const std::string &message);
```

Callback methods used to subscribe to log messages of different levels.
    
```cpp
void Logger::subscribeCallback(LogLevel type, const std::string &name, std::function<void(const std::string &)> callback);
void Logger::unsubscribeCallback(LogLevel type, const std::string &name);
```

Methods used to change and retrieve the current log level.

```cpp
void Logger::setLogLevel(LogLevel logLevel);
LogLevel Logger::getLogLevel() const;
```

### Examples
