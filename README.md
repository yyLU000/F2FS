# F2FS_ubuntu_linux
```bash
sudo apt update && sudo apt upgrade -y
```

```bash
sudo apt install -y build-essential libncurses-dev bison flex libssl-dev libelf-dev bc  
sudo apt install -y fakeroot kernel-package cpio zstd  
sudo apt install -y f2fs-tools  # F2FS 文件系统工具
```
```bash
cd /usr/src
wget https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.15.139.tar.xz
tar -xvf linux-5.4.290.tar.xz
cd linux-5.4.290
```
```bash
cp -v /boot/config-$(uname -r) .config
make menuconfig
uname -r  # 查看版本
```
```bash
make -j$(nproc)
make modules_install
make install
```
```bash
sudo update-grub
sudo reboot
```
```bash
dd if=/dev/zero of=f2fs.img bs=1M count=1024  # 创建 1GB 文件
mkfs.f2fs f2fs.img
```

# Qemu
```bash
brew install qemu
qemu-system-x86_64 --version
curl -LO https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.4.290.tar.xz
tar -xvf linux-5.4.290.tar.xz
cd linux-5.4.290
# aarch64交叉编译工具链
brew tap messense/macos-cross-toolchains
brew install aarch64-unknown-linux-gnu
# 环境变量
export CROSS_COMPILE=aarch64-unknown-linux-gnu-
export ARCH=arm64
make defconfig
make menuconfig   # F2FS, KGDB, DEBUG_FS

```

```bash
curl -LO https://busybox.net/downloads/busybox-1.35.0.tar.bz2
tar -xvf busybox-1.35.0.tar.bz2
cd busybox-1.35.0
```

编译BusyBox

```bash
make defconfig
make menuconfig

qemu-system-aarch64 \
  -M virt -cpu cortex-a53 -m 4096 \
  -nographic \
  -kernel output/images/Image \
  -append "root=/dev/vda rw console=ttyAMA0" \
  -drive file=output/images/rootfs.ext4,format=raw,if=virtio
  -drive file=f2fs.img,format=raw,if=virtio


//to locate(not work)
qemu-system-aarch64 \
  -machine virt -cpu cortex-a57 -nographic \
  -m 1024 \
  -kernel output/images/Image \
  -append "root=/dev/vda console=ttyAMA0" \
  -drive if=none,file=output/images/rootfs.ext2,format=raw,id=hd0 \
  -device virtio-blk-device,drive=hd0 \
  -drive if=none,file=f2fs.img,format=raw,id=hd1 \
  -device virtio-blk-device,drive=hd1

successful! :

CUSTOM_TARBALL -> file:///usr/src/linux-5.4.290.tar.xz
DEFCONFIG -> virt


sudo FORCE_UNSAFE_CONFIGURE=1 make

10.164.247.159


qemu-system-aarch64 \
  -M virt -cpu cortex-a53 -m 4096 \
  -nographic \
  -kernel output/images/Image \
  -append "root=/dev/vda rw console=ttyAMA0" \
  -drive file=output/images/rootfs.f2fs,format=raw,if=virtio


dd if=/dev/zero of=testfile bs=1M count=50
cat /sys/fs/f2fs/vda/gc_*

```
