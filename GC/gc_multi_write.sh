set -e

MOUNT_POINT="/mnt/f2fs-test"
DEVICE="/dev/vdb"  
FILE="$MOUNT_POINT/test1"

NUM_FILES=4
FILE_SIZE="1G"
TOTAL_SIZE="4G"
RUNTIME=15
FRAGMENT_ROUNDS=10

TRACE_LOG="/tmp/gc_log.txt"

echo "[+] Unmounting previous mount (if any)..."
umount $MOUNT_POINT 2>/dev/null || true

echo "[+] Formatting $DEVICE as F2FS (8GB)..."
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

echo "[+] Starting multi-file sequential write test..."
echo "[+] Creating $NUM_FILES files with ${FILE_SIZE} each (Total: ${TOTAL_SIZE})"

# 多文件异步顺序写入
fio --name=multi_seq_write \
    --directory="$MOUNT_POINT" \
    --nrfiles=1 \
    --filesize="$FILE_SIZE" \
    --filename_format='testfile_$jobnum' \
    --rw=write \
    --bs=128K \
    --ioengine=psync \
    --iodepth=1 \
    --numjobs="$NUM_FILES" \
    --direct=1 \
    --group_reporting \
    --thread \
    --create_on_open=1

echo "[+] File count: $NUM_FILES"
echo "[+] Individual file size: $FILE_SIZE"
echo "[+] Total data size: $TOTAL_SIZE"

for round in $(seq 1 $FRAGMENT_ROUNDS); do
    echo "[+] Fragmentation round $round/$FRAGMENT_ROUNDS"
    fio --name=fragment_creation \
        --directory="$MOUNT_POINT" \
        --nrfiles=1 \
        --filename_format='testfile_$jobnum' \
        --filesize=256M \
        --rw=randwrite \
        --bs=4K \
        --ioengine=libaio \
        --iodepth=16 \
        --numjobs="$NUM_FILES" \
        --runtime=15 \
        --direct=1 \
        --group_reporting \
        --thread \
        --time_based \
        --percentage_random=80 \
        --norandommap
done
echo "[+] Fragmentation process completed!"

# 显示文件信息
echo "[+] File listing:"
ls -lh "${MOUNT_POINT}"/testfile_* 2>/dev/null | head -10

if command -v filefrag >/dev/null 2>&1; then
    echo "[+] Checking file fragmentation:"
    for file in "${MOUNT_POINT}"/testfile_*; do
        if [ -f "$file" ]; then
            echo "$(basename "$file"): $(filefrag "$file" | grep -o '[0-9]* extent' | head -1)"
        fi
    done | head -5
fi

echo "[+] Multi-file fragmented write test completed!"
echo "[+] Use the sequential read script to test read performance on fragmented files."

echo "[+] Stopping trace_pipe logging..."
kill $TRACE_PID

echo "[+] Evaluating ... *** +++ !!!"
cat /sys/fs/f2fs/vdb/dirty_segments | tee -a $LOG
cat /sys/fs/f2fs/vdb/free_segments | tee -a $LOG

echo "[+] Status~..."
cat /sys/kernel/debug/f2fs/status
echo "[+] Done."
