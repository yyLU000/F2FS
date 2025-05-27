MOUNT_POINT="/mnt/f2fs-test"
DEVICE="/dev/vdb"  
FILE="$MOUNT_POINT/test1"
NUM_FILES=4
RUNTIME=30

df -h $MOUNT_POINT

echo "[+] Dropping caches before read test..."
sync
echo 3 > /proc/sys/vm/drop_caches

echo "[+] Count Read I/O..."
cat /sys/block/vdb/stat

echo "[+] Starting sequential read test after fragmentation..."

fio --name=seq_read_large \
    --directory="$MOUNT_POINT" \
    --filename_format='testfile_0' \
    --filesize=512M \
    --rw=read \
    --bs=128K \
    --ioengine=sync \
    --numjobs=1 \
    --direct=1 \
    --group_reporting \
    --thread

wait
sync
echo "[+] Sequential read test complete."

echo "[+] Count Read I/O..."
cat /sys/block/vdb/stat

echo "[+] DONE!"
