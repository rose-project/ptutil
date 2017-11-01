# ptutil
Partitiontable manipulation utility is created to do modifications to
an already existing partition table. For embedded devices often it is
needed to have a primary and a backup root file system. To switch between
them in case of a software update or a corrupted primary system without an user
interaction, this tool could help.

## Table of Contents
- [What's new](#whats-new)
- [Dependencies](#dependencies)
- [How to build](#how-to-build)
- [Example](#example)

## Versioning

```
major.minor.patch
```

## What's new

###### Version 0.1.0: First version for gpt manipulation (x64 tested only)
* gpt header validation and invalidation
* dumping gpt and partition entries

## Dependencies

* gcc >= 4.7.3 (c11 support)
* GTest (optional)

## How to build

```
mkdir build && cd build
cmake ..
make
```

## Example

```
# To validate primary gpt:
./ptutil --primary --validate ./disk.img

# Invalidates the primary gpt:
./ptutil --primary --invalidate ./disk.img

# To repair an invalidated gpt header call:
./ptutil --primary --validate -r ./disk.img
```

To dump informations:
```
./ptutil --primary -d ./disk.img
```
