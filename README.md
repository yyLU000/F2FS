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
cat > !/bin/sh/ << "EOF"
#!/bin/sh

echo "=== F2FS GC 实验开始 ==="

echo "[1] 创建大量小文件..."
i=0
while true; do
    dd if=/dev/zero of=/test/file_$i bs=4K count=256 conv=fsync 2>/dev/null
    i=$((i + 1))
    used=$(df /test | awk 'NR==2 {print $5}' | sed 's/%//')
    if [ "$used" -gt 95 ]; then
        echo "磁盘使用率达到 $used%，停止写入"
        break
    fi
done

echo "[2] 删除部分文件..."
for j in $(seq 0 $((i / 2))); do
    rm -f /test/file_$j
done
sync

echo "[3] 使用 fio 写入触发 GC..."
fio --name=gc_test \
    --directory=/test \
    --size=100M \
    --time_based --runtime=60s \
    --rw=write \
    --bs=4k \
    --ioengine=sync \
    --direct=1 \
    --numjobs=1

echo "[4] 当前 GC 状态："
cat /sys/kernel/debug/f2fs/status 2>/dev/null | grep -i gc

echo "=== F2FS GC 实验结束 ==="
EOF
```

```bash
#!/bin/sh
set -e

echo "=== F2FS GC 实验开始 ==="

# 挂载位置和设备
DEV=/dev/vda
MNT=/mnt/f2fs_test
mkdir -p $MNT

echo "[1] 格式化并挂载 F2FS（如果需要）..."
mkfs.f2fs -f $DEV
mount -t f2fs $DEV $MNT

echo "[2] 初始写入小文件填满磁盘..."
i=0
while true; do
  dd if=/dev/zero of=$MNT/file_$i bs=4K count=1 conv=fsync status=none || break
  i=$((i + 1))
done
echo "共写入 $i 个文件。磁盘已满。"

echo "[3] 删除一部分小文件制造无效块（给 GC 提供机会）..."
j=0
while [ $j -lt $i ]; do
  rm -f $MNT/file_$j
  j=$((j + 5))  # 每隔5个删1个
done
sync
sleep 1

USED_KB=$(df $MNT | awk 'NR==2{print $3}')
AVAIL_KB=$(df $MNT | awk 'NR==2{print $4}')
echo "剩余空间：$AVAIL_KB KB，可用：$(awk "BEGIN {printf \"%.2f\", $AVAIL_KB/($USED_KB+$AVAIL_KB)*100}")%"

echo "[4] 使用 fio 顺序写入以诱发 GC..."
fio --name=gc_test \
    --directory=$MNT \
    --size=80M \
    --rw=write --bs=4k \
    --ioengine=sync --direct=1 \
    --runtime=30s --time_based \
    --numjobs=1 --group_reporting

echo "[5] 查看 F2FS 内核 GC 日志："
dmesg | grep -i f2fs | tail -n 50

echo "[6] 清理环境（可选）"
umount $MNT

echo "=== F2FS GC 实验结束 ==="

```




#!/bin/sh
set -e

echo "=== F2FS GC 实验开始 ==="

# 挂载位置和设备
DEV=/dev/vdb
MNT=/mnt/f2fs_test
mkdir -p $MNT

echo "[1] 格式化并挂载 F2FS（如果需要）..."
mount -t f2fs -o gc_debug,discard,no_heap,victim_sel_policy=0 /dev/vdb /mnt/f2fs_test

echo "[2] 初始写入小文件填满磁盘..."
i=0
while true; do
  dd if=/dev/zero of=$MNT/file_$i bs=4K count=1 conv=fsync status=none || break
  i=$((i + 1))
done
echo "共写入 $i 个文件。磁盘已满。"

echo "[3] 删除一部分小文件制造无效块（给 GC 提供机会）..."
j=0
while [ $j -lt $i ]; do
  rm -f $MNT/file_$j
  j=$((j + 5))  # 每隔5个删1个
done
sync
sleep 1

USED_KB=$(df $MNT | awk 'NR==2{print $3}')
AVAIL_KB=$(df $MNT | awk 'NR==2{print $4}')
echo "剩余空间：$AVAIL_KB KB，可用：$(awk "BEGIN {printf \"%.2f\", $AVAIL_KB/($USED_KB+$AVAIL_KB)*100}")%"

echo "[4] 使用 fio 顺序写入以诱发 GC..."
fio --name=gc_test --filename=/mnt/f2fs_test/gc_test \
    --size=80m --bs=4k --rw=write --ioengine=sync \
    --rate_iops=100 --direct=1 --numjobs=1


echo "[5] 查看 F2FS 内核 GC 日志："
dmesg | grep -i f2fs | tail -n 50

echo "[6] 清理环境（可选）"
umount $MNT

echo "=== F2FS GC 实验结束 ==="
