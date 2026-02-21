## **Fossil Data by Fossil Logic**

**Fossil Data** is a lightweight, cross-platform data science and analytics library written in pure C with zero external dependencies. Designed for performance-critical, embedded, and trust-sensitive environments, Fossil Data provides a foundational toolkit for structured data processing, statistical analysis, numerical computation, feature extraction, and dataset transformation—prioritizing transparency, portability, and long-term stability.

Fossil Data serves as a low-level core upon which higher-level analytics, AI, and learning systems can be built without reliance on heavyweight runtimes or opaque frameworks.

### Key Features

- **Cross-Platform & Embedded-Ready**  
  Runs consistently on Windows, macOS, Linux, and constrained embedded targets.

- **Core Data Science Primitives**  
  Includes typed datasets, statistical functions, numerical methods, vectorized operations, aggregations, and transformation utilities.

- **Zero External Dependencies**  
  Implemented entirely in portable C to ensure auditability, reproducibility, and long-term maintainability.

- **Performance-First Design**  
  Optimized for low memory usage, deterministic execution, and high throughput on both modern and resource-limited systems.

- **Modular Architecture**  
  Designed as a reusable core for domain-specific sub-libraries such as machine learning, analytics pipelines, visualization backends, and AI systems within the Fossil Logic ecosystem.

## ***Prerequisites***

To get started, ensure you have the following installed:

- **Meson Build System**: If you don’t have Meson `1.8.0` or newer installed, follow the installation instructions on the official [Meson website](https://mesonbuild.com/Getting-meson.html).

### Adding Dependency

#### Adding via Meson Git Wrap

To add a git-wrap, place a `.wrap` file in `subprojects` with the Git repo URL and revision, then use `dependency('fossil-data')` in `meson.build` so Meson can fetch and build it automatically.

#### Integrate the Dependency:

Add the `fossil-data.wrap` file in your `subprojects` directory and include the following content:

```ini
[wrap-git]
url = https://github.com/fossillogic/fossil-data.git
revision = v0.1.0

[provide]
dependency_names = fossil-data
```

**Note**: For the best experience, always use the latest releases. Visit the [releases](https://github.com/fossillogic/fossil-data/releases) page for the latest versions.

## Build Configuration Options

Customize your build with the following Meson options:
	•	Enable Tests
To run the built-in test suite, configure Meson with:

```sh
meson setup builddir -Dwith_test=enabled
```

### Tests Double as Samples

The project is designed so that **test cases serve two purposes**:

- ✅ **Unit Tests** – validate the framework’s correctness.  
- 📖 **Usage Samples** – demonstrate how to use these libraries through test cases.  

This approach keeps the codebase compact and avoids redundant “hello world” style examples.  
Instead, the same code that proves correctness also teaches usage.  

This mirrors the **Meson build system** itself, which tests its own functionality by using Meson to test Meson.  
In the same way, Fossil Logic validates itself by demonstrating real-world usage in its own tests via Fossil Test.  

```bash
meson test -C builddir -v
```

Running the test suite gives you both verification and practical examples you can learn from.

## Contributing and Support

For those interested in contributing, reporting issues, or seeking support, please open an issue on the project repository or visit the [Fossil Logic Docs](https://fossillogic.com/docs) for more information. Your feedback and contributions are always welcome.
