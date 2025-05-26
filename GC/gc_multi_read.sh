MOUNT_POINT="/mnt/f2fs-test"
DEVICE="/dev/vdb"  
FILE="$MOUNT_POINT/test1"
NUM_FILES=4

df -h $MOUNT_POINT

echo "[+] Dropping caches before read test..."
sync
echo 3 > /proc/sys/vm/drop_caches

echo "[+] Starting sequential read test after fragmentation..."

for i in $(seq 1 $NUM_FILES); do
  fio --name=seq_read_$i \
      --filename=${FILE_PREFIX}_$i \
      --size=512M \
      --rw=read \
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
echo "[+] Sequential read test complete."

echo "[+] Count Read I/O..."
cat /sys/block/vdb/stat

echo "[+] DONE!"
