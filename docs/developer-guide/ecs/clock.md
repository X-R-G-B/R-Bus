## Clock

A clock can be used to measure time.

You need to create a clock, it will give you the ID to retrieve it and get ellapsed time

### Methods

```cpp
std::size_t Clock::create();
std::size_t Clock::elapsedSecondesSince(std::size_t id); // 1
std::size_t Clock::elapsedMillisecondsSince(std::size_t id); // 10^-3
std::size_t Clock::elapsedNanosecondsSince(std::size_t id); // 10^-9
void Clock::restart(std::size_t id);
```

### Example

```cpp
#include "Registry.hpp"

std::size_t id = Registry::getInstance().getClock().create();
std::cout << Registry::getInstance().getClock().elapsedMillisecondsSince(id) << std::endl;
Registry::getInstance().getClock().restart(id);
```
