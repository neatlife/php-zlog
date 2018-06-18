# zlog Extension for PHP

A reliable, high-performance, thread safe, flexsible, clear-model write by c extension for php.

## Build from sources

``` bash
% git clone --recursive git@github.com:neatlife/php-ext-zlog.git
% cd php-ext-zlog
% phpize
% ./configure
% make
% make install
```

To use the system library

``` bash
% ./configure --with-zlog
```

## Configration

zlog.ini:

```
extension=zlog.so
```

## Function

- [x] zlog\_init  — init zlog logger
- [x] zlog\_get\_category — get a log category
- [x] zlog\_info — log a info string
- [ ] zlog\_warn — log a warn string
- [ ] zlog\_error — log a error string
- [ ] zlog\_notic — log a notic string
- [ ] zlog\_debug — log a debug string
- [ ] zlog\_fatal — log a fatal string

### zlog\_init

bool $isSuccess **zlog\_init** (string \$logConfig)

```php
var_dump(zlog_init(__DIR__ . "/zlog.conf"));
```

```ini
# zlog.conf
[formats]
simple = "%m%n"
[rules]
my_cat.DEBUG    >stdout; simple
```

### zlog\_get\_category

resource $category  **zlog\_get\_category** (string \$category )

```php
var_dump(zlog_init(__DIR__ . "/zlog.conf"));

var_dump(zlog_get_category("my_cat"));
```

return category resource represent success or false on failure

### zlog\_info

NULL **zlog\_info** ( \$log )

```php
zlog_info("hello world");
```

log a info string

## Examples

```php
// zlog_init()
var_dump(zlog_init(__DIR__ . "/zlog.conf"));
```

```php
// zlog_get_category()
var_dump(zlog_init(__DIR__ . "/zlog.conf"));

var_dump(zlog_get_category("hello world"));
```

```php
// zlog_info()
var_dump(zlog_init(__DIR__ . "/zlog.conf"));

var_dump(zlog_get_category("my_cat"));

zlog_info("hello world");
```
