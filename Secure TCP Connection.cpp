#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>


class PcapCapture {

   private: pcap_t * handle;
   struct bpf_program fp;
   bpf_u_int32 net;
   bpf_u_int32 mask;
   char errbuf[PCAP_ERRBUF_SIZE];

   public: PcapCapture() {
      handle = NULL;
      net = 0;
      mask = 0;
      memset(errbuf, 0, sizeof(errbuf));
   }

   void find_default_interface() {
      if (pcap_lookupnet(pcap_lookupdev(errbuf), & net, & mask, errbuf) == -1) {
         fprintf(stderr, "pcap_lookupnet: %s\n", errbuf);
         exit(1);
      }
   }

   void open_promiscuous_mode() {
      if ((handle = pcap_open_live(pcap_lookupdev(errbuf), SNAP_LEN, 1, 1000, errbuf)) == NULL) {
         fprintf(stderr, "pcap_open_live: %s\n", errbuf);
         exit(1);
      }
   }

   void compile_filter_expression(const char * filter_exp) {
      if (pcap_compile(handle, & fp, filter_exp, 0, net) == -1) {
         fprintf(stderr, "pcap_compile: %s\n", pcap_geterr(handle));
         exit(1);
      }
   }

   void set_filter_expression() {
      if (pcap_setfilter(handle, & fp) == -1) {
         fprintf(stderr, "pcap_setfilter: %s\n", pcap_geterr(handle));
         exit(1);
      }

      void start_capture() {
         pcap_loop(handle, -1, pcap_callback, NULL);
      }

      void close() {
         pcap_close(handle);
      }

      // callback function
      static void pcap_callback(
         u_char * args, const struct pcap_pkthdr * header, const u_char * packet
      ) {
         printf("Packet length: %d\n", header->len);
      }

   };

   int main(int argc, char ** argv) {
      PcapCapture capture;
      capture.find_default_interface();
      capture.open_promiscuous_mode();
      capture.compile_filter_expression("src host !localhost && dst port 4444");
      capture.set_filter_expression();
      capture.start_capture();
      capture.close();

      return 0;

   }
