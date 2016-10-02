TODO
====

Version 0.1 (in progress):
--------------------------

### Milestone 1

- [x] CMake Buildsystem
- [x] Basic De-/Init of GPT Header structure
- [x] In-/Validate GPT Header
- [x] Dump GPT Header

### Milestone 2
- [X] Read and validate partition entries
- [X] Dump partiton entries

### Milestone 3
- [ ] Write Unit-Tests
- [ ] Correct error handling for EBUSY, EAGAIN etc...
- [ ] PTUtility executable
- [ ] API rework and documentation

Version 0.2 (planning):
-----------------------

### Milestone 1
- [ ] Capability to move gpt & partition entries (e.g. if gpt not written at end of partition)
- [ ] In-/Validation of single partition entries

Version 0.3 (future):
-----------------------

### Milestone 1
- [ ] cross compile for arm and x86/x64
