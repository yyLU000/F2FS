set -e

MOUNT_POINT="/mnt/f2fs-test"
DEVICE="/dev/vdb"  
FILE="$MOUNT_POINT/test1"
NUM_FILES=4
TRACE_LOG="/tmp/gc_log.txt"

echo "[+] Unmounting previous mount (if any)..."
umount $MOUNT_POINT 2>/dev/null || true

echo "[+] Formatting $DEVICE as F2FS (5GB)..."
mkfs.f2fs -f -s 1 $DEVICE

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

echo "[+] Starting multi-file sequential write to create fragmentation..."

for i in $(seq 1 $NUM_FILES); do
  fio --name=seq_write_$i \
      --filename=${FILE_PREFIX}_$i \
      --size=512M \
      --rw=write \
      --bs=128K \
      --ioengine=libaio \
      --iodepth=16 \
      --numjobs=1 \
      --direct=1 \
      --runtime=60 \
      --group_reporting &
done

wait
sync
echo 3 > /proc/sys/vm/drop_caches
echo "[+] Initial sequential write done."

echo "[+] Starting random overwrite to simulate fragmentation..."

for round in $(seq 1 5); do
  echo "[+] Overwrite round $round"
  for i in $(seq 1 $NUM_FILES); do
    fio --name=rand_overwrite_${round}_$i \
        --filename=${FILE_PREFIX}_$i \
        --size=512M \
        --rw=randwrite \
        --bs=8K \
        --ioengine=libaio \
        --iodepth=16 \
        --numjobs=1 \
        --direct=1 \
        --runtime=30 \
        --group_reporting &
  done
  wait
done

sync
echo 3 > /proc/sys/vm/drop_caches
echo "[+] Fragmentation complete."

echo "[+] Stopping trace_pipe logging..."
kill $TRACE_PID

echo "[+] Evaluating ... *** +++ !!!"
cat /sys/fs/f2fs/vdb/dirty_segments | tee -a $LOG
cat /sys/fs/f2fs/vdb/free_segments | tee -a $LOG

echo "[+] Status~..."
cat /sys/kernel/debug/f2fs/status
echo "[+] Done."
