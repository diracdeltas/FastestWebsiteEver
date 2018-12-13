[![Build
Status](https://travis-ci.org/diracdeltas/FastestWebsiteEver.svg?branch=master)](https://travis-ci.org/diracdeltas/FastestWebsiteEver)

# what is this?

the greatest website to ever fit in a single TCP packet

# optimizations

* HTTP compression: choose DEFLATE over GZIP because most browsers accept raw DEFLATE output without a header or checksum so it's shorter/faster. #yolo
* send response immediately after TCP session init: avoids 1 round trip (not to spec, seems to confuse Chrome results in Wireshark).
* `SO_BUSY_POLL`: asks the kernel to poll for packets for a given amount of time.
* `TCP_NODELAY`: disable Nagle's algorithm.
* `echo 1 > /proc/sys/net/ipv4/tcp_low_latency`: TCP stack makes decisions that prefer lower latency as opposed to higher throughput.
* favicon: reduced gif to 1/2 original frame count, applied lossy LZW compression.
* HTML mangling: closing tags are for losers.
* bespoke webserver written in C
* remove GIF comment extension block, saving 36 bytes (thanks Samy Kamkar)

# who made this?

eV (admin@packet.city), yan (webmaster@packet.city)

Get in touch today to hear about our 120-byte ad sponsorship opportunities!

# credits

* visuals based on http://www.p01.org/128b_raytraced_checkboard/ by Mathieu 'p01' Henri
* favicon based on http://cultofthepartyparrot.com/
* thank you to [Candy Japan](https://www.candyjapan.com/) for the generous
  sponsorship! surprise Japanese candy boxes twice a month!!
