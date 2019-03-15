# CG

## Compiling (linux)
```bash
./build.sh
```

## Running
### Generator
```bash
target/release/generator/generator FILENAME OPTION [PARAMS...]
```

### Engine
```bash
target/release/engine/engine [SCENE]
```
If no scene is provided `config.xml` will be used.

### Help
Both programs support a `--help` flag to provide some guidance on how to use them.
