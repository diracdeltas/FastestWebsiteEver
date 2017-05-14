# what is this?

the greatest website to ever fit in a single TCP packet

# optimizations

* HTTP compression: choose DEFLATE over GZIP because most browsers accept raw DEFLATE output without a header or checksum. #yolo
* send response immediately after TCP session init: avoids 1 round trip (not to spec).
* `SO_BUSY_POLL`: asks the kernel to poll for packets for a given amount of time.
* `echo 1 > /proc/sys/net/ipv4/tcp_low_latency`: TCP stack makes decisions that prefer lower latency as opposed to higher throughput.
* favicon: reduced gif to 1/2 original frame count, applied lossy LZW compression.
* HTML mangling: closing tags are for losers.
* bespoke webserver written in C

# who made this?

eV (admin@packet.city), yan (webmaster@packet.city)

Get in touch today to hear about our 150-byte ad sponsorship opportunities!

# credits

* visuals based on http://www.p01.org/128b_raytraced_checkboard/ by Mathieu 'p01' Henri
* favicon based on http://cultofthepartyparrot.com/
