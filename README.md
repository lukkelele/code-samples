# code samples :stars:

> Code samples to help my older brother learn programming :smile:

---

## Setup
Begin by cloning the repo.

```
git clone https://github.com/lukkelele/code-samples.git
```

### Devcontainer
GUI samples need access to the GPU.

#### Nvidia GPU
Make sure the container has access, run on host:
```
docker info | grep -i runtime
docker run --rm --gpus all ubuntu:24.04 nvidia-smi
```

Check if nvidia-ctk is installed.
```
which nvidia-ctk
```
Otherwise, install it and configure docker.
```
sudo nvidia-ctk runtime configure --runtime=docker
sudo systemctl restart docker
```

The devcontainer.json *might* need the `--gpus=all` argument to run.

<br>

---

## Samples
> - [C](./samples/c)
> - [C++](./samples/cpp)

There are tasks available to use to build and run each sample with Zed/Visual Studio Code.

<br>

## Build a sample using Tasks
> The following is for C and C++ samples.

#### Zed
1. Open a file in the sample directory you wish to build
1. Open the **Command Palette** (Ctrl + Shift + P)
1. Select **task: spawn**
1. Select **cmake: configure** (if not configured yet)
1. Build by using **cmake: build**
1. Run by using **sample: run**

#### Visual Studio Code
1. Open a file in the sample directory you wish to build
1. Open the **Command Palette** (Ctrl + Shift + P)
1. Select **Tasks: Run Task**
1. Select **cmake: configure** (if not configured yet)
1. Build by using **cmake: build**
1. Run by using **sample: run**

When you have run **cmake: configure** and the build folder hasn't been removed/modified elsewhere, you can just run the **cmake: build** task to re-compile. The **sample: run** task also allows for recompilation.  
Therefore it can be useful to re-run the **sample: run** task if modifying a sample and wanting to see the changes you've made in real time.

<br>

## Experimental

The directory [experimental](./experimental) contains random code I want to try out and experiment with.

---

## TODO
:white_check_mark: Tasks for Zed/Visual Studio Code  
:black_square_button: Devcontainer  
:black_square_button: Setup script for checking dependencies if devcontainer not used
