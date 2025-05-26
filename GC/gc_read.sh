MOUNT_POINT="/mnt/f2fs-test"
DEVICE="/dev/vdb"  
FILE="$MOUNT_POINT/test1"

df -h $MOUNT_POINT

echo "[+] Dropping caches before read test..."
sync
echo 3 > /proc/sys/vm/drop_caches

echo "[+] Starting sequential read test..."
fio --name=read_test \
    --filename=$FILE \
    --size=2G \
    --rw=read \
    --bs=128K \
    --ioengine=sync \
    --numjobs=1 \
    --direct=1 \
    --group_reporting

echo "[+] Dropping caches again..."
sync
echo 3 > /proc/sys/vm/drop_caches

echo "[+] Starting random read test..."
fio --name=read_test \
    --filename=$FILE \
    --rw=randread \
    --bs=8K \
    --ioengine=libaio \
    --iodepth=16 \
    --numjobs=4 \
    --direct=1 \
    --runtime=60 \
    --group_reporting

echo "[+] Count Read I/O..."
cat /sys/block/vdb/stat

echo "[+] DONE!"
