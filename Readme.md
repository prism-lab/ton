Telegram Open Network repository. Currently maintained by ton-rocks team.



# Academic and Theory:
[TON Whitepaper](https://ton.org/ton.pdf) – a general description of TON Network and TON Blockchain.

[TON Blockchain description](https://ton.org/tblkch.pdf) (may include outdated information).

[Description of Catchain BFT Consensus protocol](https://ton.org/catchain.pdf) employed by TON Blockchain while creating new blocks.

[TON Virtual Machine description](https://ton.org/tvm.pdf)

[A brief introduction to the Fift programming language](https://ton.org/fiftbase.pdf)

Notice: _All of the papers above can be found in the `/docs` section of this repository._

# Installation:

## on Linux:

1. Collect all the necessary dependencies:

    | Dependency:   | Min. version | Debian/Ubuntu/Mint pkg | Fedora pkg            | Arch pkg     |
    |---------------|--------------|------------------------|-----------------------|--------------|
    | Git           | any          | `git`                  | `git`                 | `git`        |
    | Cmake         | 3.0.2        | `cmake`                | `cmake`               | ?            |
    | Dev Tools     | 7.2 ?        | `build-essential`      | `Development Tools`   | `base-devel` |
    | OpenSSL       | 1.1          | `libssl-dev`           | `openssl-devel`       | ?            |
    | zlib          | ?            | `zlib1g-dev`           | `zlib-devel`          | ?            |
    | Libmicrohttpd | ?            | `libmicrohttpd-dev`    | `libmicrohttpd-devel` | ?            |

    - Debian / Ubuntu / Mint one liner:
        ```
        sudo apt-get install git build-essential cmake libssl-dev zlib1g-dev libmicrohttpd-dev 
        ```
   
    - Fedora / CentOS one liner:
        ```
        dnf groupinstall 'Development Tools' -y && dnf install -y git gcc cmake openssl-devel zlib-devel libmicrohttpd-devel
        ```

1. Clone this repository:
    ```
    git clone --recursive https://github.com/ton-rocks/ton
    ```

1. Build the source code:
    ```
    cd ton
    mkdir build && cd build
    cmake ..
    make
    ```

1. Optionally you may want to install generated binaries on your system:
    ```
    make install
    ```

## on Windows:

1. Install [Visual Studio 2019](https://visualstudio.microsoft.com/vs/downloads/) with ```Desktop development with C++``` package.

1. Install ```Git``` either through [official release](https://git-scm.com/downloads) either from Visual Studio's individual components section.

1. Download and install [vcpkg package manager](https://github.com/microsoft/vcpkg) with the following console commands:
    ```
    git clone https://github.com/microsoft/vcpkg
    cd vcpkg
    .\bootstrap-vcpkg.bat
    ```

1. Install dependencies:
    ```
    .\vcpkg.exe install openssl-windows zlib libmicrohttpd
    cd ..
    ```

1. Clone this repository: 
    ```
    git clone --recursive https://github.com/ton-rocks/ton
    ```

1. Generate Visual Studio solution files:
    ```
    cd ton
    mkdir build 
    cd build
    cmake -G "Visual Studio 16 2019" -DCMAKE_TOOLCHAIN_FILE=..\..\vcpkg\scripts\buildsystems\vcpkg.cmake
    ```

1. Open the generated solution and build the project.

## on Mac:
:construction: **This section is under development.** :construction:

1. Install Xcode developer tools and [vcpkg](https://github.com/Microsoft/vcpkg)
1. Install all relevant dependencies:
    ```
    vcpkg install openssl zlib libmicrohttpd
    ```

1. Install cmake
    ```
    brew install cmake
    ```

1. Clone this repository: 
    ```
    git clone --recursive https://github.com/ton-rocks/ton
    ```

1. Compile:
    ```
    cd ton
    mkdir build
    cd build
    cmake ..
    make
    ```
  