
set -e

MOUNT_POINT="/mnt/f2fs-test"
DEVICE="/dev/vdb"  
FILE="$MOUNT_POINT/test1"
TRACE_LOG="/tmp/gc_log.txt"

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

echo "[+] Starting trace_pipe to log GC events..."
# 启动 trace_pipe 到后台，并保存 PID
cat /sys/kernel/debug/tracing/trace_pipe > $TRACE_LOG &
TRACE_PID=$!
sleep 1

echo "[+] Starting sequential write test..."
fio --name=seq_write \
    --filename=$FILE \
    --size=3G \
    --rw=write \
    --bs=128K \
    --ioengine=sync \
    --numjobs=1 \
    --runtime=60 \
    --direct=1 \
    --group_reporting

echo "[+] Starting random update test..."
fio --name=rand_update \
    --filename=$FILE \
    --size=1G \
    --rw=randwrite \
    --bs=4K\
    --ioengine=sync \
    --numjobs=4 \
    --runtime=300 \
    --direct=1 \
    --time_based \
    --group_reporting

echo "[+] Stopping trace_pipe logging..."
kill $TRACE_PID

echo "[+] Evaluating ... *** +++ !!!"
cat /sys/fs/f2fs/vdb/dirty_segments | tee -a $LOG
cat /sys/fs/f2fs/vdb/free_segments | tee -a $LOG

echo "[+] Done."
