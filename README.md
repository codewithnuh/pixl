# pixl

High-speed image tools for the command line.

`pixl` is a C++-based image processing CLI focused on fast local workflows. The current code can load an image, inspect it with `stb_image`, and print basic metadata such as dimensions, channels, format extension, and file size.

This project is in active development. The current implementation is an early foundation, and the roadmap below reflects the planned path from a simple MVP to a more complete image processing toolkit.

## Motivation

- Why should basic image inspection require opening a heavy GUI app?
- Why should simple tasks like checking dimensions or file size need a full editor?
- Why should converting or resizing images depend on slow, repetitive manual steps?
- Why should batch image work be harder than running a single command?

`pixl` is being built to remove that friction and make common image tasks fast, scriptable, and easy to use from the terminal.

## Current status

- Reads image metadata from a file path
- Prints width, height, channels, file type, and file size
- Uses `stb_image` and `stb_image_write`
- Includes Node.js addon dependencies in the repo setup, but the main implemented workflow is currently the native C++ CLI

## Project goals

The goal is to build a fast and practical image utility that starts with a small CLI and grows into a broader tool for conversion, resizing, optimization, batch processing, and developer workflows.

## MVP goal

The first milestone is a useful command-line tool with these core features:

- `pixl info photo.jpg`
- `pixl convert input.png output.jpg`
- `pixl resize input.jpg output.jpg 800x600`
- Helpful error handling for missing files, unsupported formats, and overwrite protection

### MVP success criteria

- Cross-platform support for Windows, macOS, and Linux
- Process a 5 MB image in under 0.5 seconds
- Support PNG, JPG, and BMP
- Provide a clear `--help` command

## Roadmap

### Level 1: Basic operations

- Image information: dimensions, channel detection, file size, bit depth, compression info, format detection
- Format conversion: PNG/JPG/BMP conversion, batch conversion, basic metadata preservation

### Level 2: Image manipulation

- Resizing: exact size, aspect-ratio aware resize, percentage scaling, fit/cover modes, multiple output sizes
- Color operations: grayscale, color depth changes, JPG quality control, alpha strip/add support

### Level 3: Advanced features

- Archive and container tooling: ICO generation, ZIP output, sprite sheets, WebP support
- Performance and batch workflows: parallel processing, watch mode, validation, progress display
- Editing helpers: rotate, flip, crop, padding, smart thumbnails

### Level 4: Power user features

- Optimization: lossless PNG compression, progressive JPG, metadata stripping, profile cleanup
- Analysis and reporting: image compare, JSON manifest generation, duplicate detection, corruption detection
- Developer utilities: base64 encode/decode, placeholder generation, GIF frame extraction, favicon packs, retina variants

### Level 5: Production readiness

- Node.js integration: N-API addon, promise API, streaming support, memory-efficient processing, progress callbacks
- Distribution: npm package, standalone executable, Docker image, GitHub Actions CI/CD
- Enterprise-focused extensions: PSD/HEIC/RAW support, watermarking, EXIF editing, smart cropping, ML upscaling

## Proposed CLI shape

```bash
pixl <command> [options] <input> [output]

pixl info image.jpg
pixl convert input.png output.jpg
pixl resize input.jpg output.jpg 800x600
pixl batch convert ./folder/ --to png
pixl ico icon.png --sizes 16,32,48
pixl zip output.zip image1.jpg image2.png
pixl optimize image.jpg --quality 85
pixl compare image1.jpg image2.jpg
pixl monitor ./watch/ --to jpg
```

### Global options

- `--help`
- `--version`
- `--verbose`
- `--quiet`
- `--force`
- `--jobs 4`

## 6-week development plan

### Week 1

- Finalize `info`
- Add `convert`
- Add `resize`
- Improve CLI argument parsing and error handling

### Week 2

- Add ICO generation
- Add ZIP archive support
- Add batch processing
- Add grayscale conversion

### Week 3

- Add parallel processing
- Add WebP support
- Add optimization flags
- Add progress indicators

### Week 4

- Build Node.js integration
- Add a JavaScript wrapper
- Implement streaming support
- Add TypeScript definitions

### Week 5

- Expand documentation
- Add cross-platform builds
- Write tests
- Prepare npm publishing

### Week 6

- Add watch mode
- Add smart cropping
- Add retina image generation
- Tune performance

## Priorities

- P0: info display, format conversion, resize
- P1: ICO generation, batch processing, ZIP support
- P2: grayscale, WebP, watch mode
- P3: face detection, RAW support

## Current code location

- Main CLI source: `src/main.cpp`
- Third-party image headers: `vendor/stb/`

## Build notes

This repository currently includes:

- C++ source for the image CLI
- `binding.gyp` for addon-related build configuration
- `node-gyp`, `node-addon-api`, and `pnpm` metadata

You can extend the existing C++ entrypoint into a command-based CLI by adding subcommands such as `info`, `convert`, and `resize`.

## License

This project is licensed under the Apache License 2.0. See the `LICENSE` file for details.
