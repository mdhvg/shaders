# Compute Shaders

This repository contains a collection compute shader programs I made as a fun practice.

## Programs list

### Falling Sand simulation

Learnt from [this video](https://youtu.be/8Tf18MMZ-5U) by [Yusef28
](https://www.youtube.com/@Yusef28) and [this paper](https://arxiv.org/abs/2008.06341)

Run `cmake -DPROGRAMS=sand ..` to build

![falling sand preview](images/sand.png)

## Building Instructions

- Recursively clone this repo

```bash
git clone --recursive https://github.com/mdhvg/compute-shaders
cd compute-shaders
```

- Create build directory

```bash
mkdir build
cd build
```

- Build a specific program

```bash
cmake -DPROGRAMS=<prog1;prog2> ..
make -j4
```

- Or simply build all programs

```bash
cmake -DPROGRAMS=all ..
make -j4
```