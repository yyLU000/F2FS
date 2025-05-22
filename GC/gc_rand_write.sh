MOUNT_POINT="/mnt/f2fs-test"
DEVICE="/dev/vdb"  
FILE="$MOUNT_POINT/test1"

df -h $MOUNT_POINT

echo "[+] Dropping caches before write test..."
sync
echo 3 > /proc/sys/vm/drop_caches

echo "[+] Starting random write test..."
fio --name=append_write_test \
    --filename=$FILE \
    --rw=randwrite \
    --bs=8K \
    --ioengine=sync \
    --direct=1 \
    --numjobs=1 \
    --runtime=30 \
    --group_reporting

echo "[+] DONE!"
