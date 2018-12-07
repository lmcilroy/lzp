# LZP - A streaming LZ Predictor compression tool

This is a simple streams based compression tool that uses the LZ predictor
algorithm.  It uses a hash of the last 4 bytes seen to predict the next
byte.  If the prediction is correct it saves the result as a single bit.
If the prediction failed it saves the result as a single bit plus emits the
byte to the output too.

Memory requirements are good, it needs 64KB for the hashtable but doesn't
need a sliding window of buffered data.  Compression ratios are generally
not great and it can often expand data.  Some files can be recursively
compressed with further incremental gains.  The only assurance it offers is
it wont try to decompress a noncompressed stream.

Performance is reasonable given that it is using buffered I/O.

```
$ cp /usr/share/dict/words words
$ ./lzp -c < words > words.lzp
$ stat -c "%s %n" words words.lzp
4953680 words
2123946 words.lzp
$ ./lzp -d < words.lzp > words2
$ cmp -l words words2
```
