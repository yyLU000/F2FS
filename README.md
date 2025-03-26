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
