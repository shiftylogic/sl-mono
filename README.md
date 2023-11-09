# The Mono Repo
A collection of code for many projects. Can be used as a template or just collecting personal projects.

---

# Dependencies
With a strong dislike for dependency hell, I prefer to minimize the use of them, only using trusted libraries when necessary. I definitely do not want to depend on a "magic" tool to manage compiling / finding dependencies. As such, all dependencies reside (in some form) within this repo (mostly in _vendor root folder). Specific versions are compiled along with the repo.

**_Updates are explicit._**

| Name | Version |
| ---- |:-------:|
| Catch2 | 2.13.9 |
| GLFW | 3.3.8 |
| GLM | 0.9.9 |
| json (nlohmann) | 3.11.2 |
| libuv | 1.45.0 |
| SQLite | 3.42.0 |
| unordered_dense | 4.0.1 |
| Volk | 1.3.268.1 |
| Vulkan | 1.3.268.1 |
| Vulkan Memory Allocator | 3.1.0-development |
| Vulkan SPIR-V Reflect | 1.3.268.1 |

---

# Work Tasks
A list of items both completed and / or planned for addition into this mono repository.

### Tooling
- [x] Custom LLVM build support
- [x] Custom CMake build support

### Core
- [x] [config] JSON config files
- [ ] [config] SQLite DB config
- [ ] [config] YAML config files
- [ ] [gfx] JPEG reader / writer
- [ ] [gfx] PNG reader / writer
- [x] [io] File read wrappers
- [x] [io] File mapping (Linux / macOS)
- [ ] [io] File mapping (Windows)
- [x] [logging] Simple application logger
- [x] [mem] Templated memory allocator
- [x] [utils] Scoped deferred functions
- [x] [utils] Lazy object initialization
- [x] [utils] Version struct
- [x] [utils] Simple string formatting
- [x] [utils] Enum iteration helper template

### Application Infrastructure
- [x] [example] Bare window creation
- [x] [example] window input handling
- [x] [input] Binding Input manager
- [x] [windowing] Window wrappers

### Data
- [x] [sqlite] Database Wrapper
- [x] [sqlite] Commands
- [x] [sqlite] Transactions
- [x] [example] Simple SQLite Database Manipulation

### Eventing / Networking
- [x] [async] UV Loop
- [ ] [async] Worker queues
- [x] [events] UV Idler
- [x] [events] UV Signals
- [x] [events] UV Timers
- [ ] [networking] TCP Client
- [ ] [networking] TCP Server
- [ ] [networking] UDP Client
- [ ] [networking] UDP Server

### Vulkan
- [ ] [compute] Compute shaders / programs
- [x] [core] Resources
- [x] [core] App instance / context
- [x] [core] Debug messenger
- [x] [core] Physical device enumeration
- [x] [core] Logical devices
- [ ] [core] Shader Modules
- [ ] [core] Descriptor set layouts
- [ ] [core] Barriers
- [x] [example] Core Initialization
- [ ] [example] Simple GPU compute
- [ ] [example] Mandelbrot image via GPU compute
- [ ] [example] Spinning Cube
- [ ] [gfx] Render context
- [ ] [gfx] Materials
- [ ] [gfx] Meshes
- [ ] [gfx] Surface
- [ ] [gfx] Swapchain
- [x] [mem] VMA device buffers
- [x] [mem] VMA device images
- [x] [spv] SPIR-V Reflect

