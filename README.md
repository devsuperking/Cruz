

![Cruz Engine Logo](https://iili.io/KkA8rBa.png)

# 2D and 3D cross-platform game engine

[Cruz Engine](https://devsuperking.github.io/Cruz/) is a concept for a modern, cross-platform **2D and 3D game engine**, running in the browser and natively. The project combines **performance**, **full control over resources and code**, **advanced analytical tools**, and a **flexible scripting environment**.

## Free, open source and community-driven
- Creating game logic in **Cruza (.crz)**, **Visual Scripting**, or any language compiling to WASM.
- Performance powered by **WebGPU, OpenGL, Vulkan, DirectX**, and **WASM**.
- Optional **standalone** application with native GPU acceleration.
- Support for older browsers via **WebGL fallback**.
-  **Support for both 2D and 3D games** in one engine.
-  **AST Graph** showing all functions, including engine ones.
-  **Analytical tools** monitoring memory allocations and function "heaviness".

## Key Features

### Web-first Environment
- Run games without installation in any browser using **WebGPU**.
-  **WebGL fallback** for older browsers.
- Dynamic module loading via **WebAssembly**.

### Standalone Acceleration
- Desktop **Cruz Standalone** application.
- Native backends: **OpenGL**, **Vulkan**, **DirectX**.

### Scripting System
-  **Cruza (.crz) language** – modern syntax inspired by Kotlin, compiled to WASM.
- Alternatively: **Visual Scripting**.
- Support for other languages compiling to WASM (Rust, AssemblyScript, C/C++, Zig).

### Graphics and Shaders

- 2D and 3D rendering in one engine.
- Backends: **WebGPU, WebGL fallback, OpenGL, Vulkan, DirectX**.
-  Shader language
- Textual or node-based
- Native GPU compilation
- Integration with rendering pipeline

  

### Performance and Analysis
- Monitor memory usage and "heaviness" of each function.
-  **AST Graph** – visualization of all project functions.
- Diagnostic tools for developers.

### Cross-platform Support
- Web, Windows, macOS, Linux
- (Planned) Android, iOS

## Cruza (.crz) Example – Kotlin-inspired Syntax
```kotlin
class Player {

	var position: Vec3 = Vec3(0, 0, 0)

	fun onStart() {
		log("Hello from Cruz Engine!")
	}

	fun onUpdate(dt: Float) {
		position.x += 2.0 * dt
	}
}
```

## Shader Example (.crzx)
```
shader BasicLit {

	uniform mat4 modelViewProjection
	input vec3 position
	input vec3 normal

	main() {
		outputColor = vec4(normal * 0.5 + 0.5, 1.0)
	}
}
```

**2D Node-based Shader:**
- Node-based: drag nodes like `Texture`, `Multiply`, `OutputColor`.
- Export to natively compiled `.crzx`.

  
## License
Cruz Engine is available under the **GPL-3.0 License**. The code will be released upon starting prototype implementation.
