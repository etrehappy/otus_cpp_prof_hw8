Allowed options:
  -h [ --help ]               produce help message
  --scan-dirs arg             directories to scan
  --exclude-dirs arg          directories to exclude from scanning
  --scan-level arg (=0)       scanning level
  --min-size arg (=1)         min file's size, byte
  --block-size arg (=5120)    block size for reading files
  --file-masks arg            file masks for comparison
  --hash-algorithm arg (=md5) hashing algorithm (crc32, md5)

  Args:
  --scan-dirs ../source --scan-level 1 --file-masks .h .cpp --exclude-dirs ../source/exclude