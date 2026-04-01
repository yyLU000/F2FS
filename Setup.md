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
  -drive file=output/images/rootfs.f2fs,format=raw,if=virtio \
  -drive file=f2fs.img, format=raw, if=virtio


dd if=/dev/zero of=testfile bs=1M count=50
cat /sys/fs/f2fs/vda/gc_*

```

```bash
fio --name=test0 \
    --directory=/test \
    --size=60M \
    --time_based --runtime=30s \
    --rw=write \
    --bs=4k \
    --ioengine=libaio \
    --direct=1 \
    --numjobs=1


test: (g=0): rw=write, bs=(R) 4096B-4096B, (W) 4096B-4096B, (T) 4096B-4096B, ioengine=sync, iodepth=1
fio-3.38
Starting 1 process
test: Laying out IO file (1 file / 60MiB)
[ 4635.668879] random: crng init doneB/s][w=4265 IOPS][eta 00m:27s]
Jobs: 1 (f=1): [W(1)][100.0%][w=18.4MiB/s][w=4700 IOPS][eta 00m:00s]
test: (groupid=0, jobs=1): err= 0: pid=161: Wed Apr 16 15:45:05 2025
  write: IOPS=4845, BW=18.9MiB/s (19.8MB/s)(568MiB/30000msec); 0 zone resets
    clat (usec): min=98, max=15705, avg=196.79, stdev=147.28
     lat (usec): min=99, max=15707, avg=198.35, stdev=147.73
    clat percentiles (usec):
     |  1.00th=[  102],  5.00th=[  131], 10.00th=[  147], 20.00th=[  159],
     | 30.00th=[  167], 40.00th=[  172], 50.00th=[  194], 60.00th=[  204],
     | 70.00th=[  206], 80.00th=[  215], 90.00th=[  229], 95.00th=[  277],
     | 99.00th=[  412], 99.50th=[  490], 99.90th=[ 1663], 99.95th=[ 3228],
     | 99.99th=[ 5473]
   bw (  KiB/s): min=15457, max=30264, per=100.00%, avg=19407.92, stdev=2811.55, samples=59
   iops        : min= 3864, max= 7566, avg=4851.85, stdev=702.95, samples=59
  lat (usec)   : 100=0.01%, 250=93.18%, 500=6.34%, 750=0.24%, 1000=0.08%
  lat (msec)   : 2=0.07%, 4=0.06%, 10=0.02%, 20=0.01%
  cpu          : usr=9.67%, sys=40.19%, ctx=142704, majf=0, minf=19
  IO depths    : 1=100.0%, 2=0.0%, 4=0.0%, 8=0.0%, 16=0.0%, 32=0.0%, >=64=0.0%
     submit    : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     complete  : 0=0.0%, 4=100.0%, 8=0.0%, 16=0.0%, 32=0.0%, 64=0.0%, >=64=0.0%
     issued rwts: total=0,145353,0,0 short=0,0,0,0 dropped=0,0,0,0
     latency   : target=0, window=0, percentile=100.00%, depth=1

Run status group 0 (all jobs):
  WRITE: bw=18.9MiB/s (19.8MB/s), 18.9MiB/s-18.9MiB/s (19.8MB/s-19.8MB/s), io=568MiB (595MB), run=30000-30000msec

Disk stats (read/write):
  vda: ios=0/144590, sectors=0/1157072, merge=0/0, ticks=0/16296, in_queue=112, util=99.74%

```



```bash
/4/23
sudo qemu-system-aarch64 -M virt -cpu cortex-a53 -m 4096   -nographic \
     -kernel output/images/Image  \
     -append "root=/dev/vda rw console=ttyAMA0" \
     -drive if=none,file=output/images/rootfs.f2fs,format=raw,id=hd0  \
     -device virtio-blk-pci,drive=hd0 \
     -drive if=none,file=f2fs.img,format=raw,id=hd1 \
     -device virtio-blk-pci,drive=hd1

**** gc.sh ****
#!/bin/sh

set -e

MOUNT_POINT="/mnt/f2fs-test"
DEVICE="/dev/vdb"   # QEMU 中第二个 virtio-blk 设备是 vdb
FILE="$MOUNT_POINT/test1"

echo "[+] Unmounting previous mount (if any)..."
umount $MOUNT_POINT 2>/dev/null || true

echo "[+] Formatting $DEVICE as F2FS (5GB)..."
mkfs.f2fs -f -s 16 $DEVICE

echo "[+] Creating mount point and mounting..."
mkdir -p $MOUNT_POINT
mount -t f2fs -o mode=lfs $DEVICE $MOUNT_POINT
df -h $MOUNT_POINT

echo "[+] Syncing and dropping caches..."
sync
echo 3 > /proc/sys/vm/drop_caches

echo "[+] Starting sequential write test..."
fio --name=seq_write \
    --filename=$FILE \
    --size=3G \
    --rw=write \
    --bs=128K \
    --ioengine=libaio \
    --numjobs=1 \
    --runtime=60 \
    --direct=1 \
    --group_reporting

echo "[+] Starting random update test..."
fio --name=rand_update \
    --filename=$FILE \
    --size=1G \
    --rw=randwrite \
    --bs=128K\
    --ioengine=sync \
    --numjobs=4 \
    --runtime=300 \
    --direct=0 \
    --time_based \
    --norandommap \
    --random_generator=lfsr \
    --group_reporting

echo "[+] Evaluating ... *** +++ !!!"
cat /sys/fs/f2fs/vdb/dirty_segments | tee -a $LOG
cat /sys/fs/f2fs/vdb/free_segments | tee -a $LOG
cat /sys/kernel/debug/f2fs/status

echo "[+] Done."


```

