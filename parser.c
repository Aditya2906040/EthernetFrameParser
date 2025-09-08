#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define ETHERTYPE_IPV4 0x0800
#define ETHERTYPE_ARP 0x0806
#define ETHERTYPE_IPV6 0x86DD
#define ETHERTYPE_VLAN 0x8100
#define ETH_HEADER_LEN 14

void getDest(const unsigned char frame[], char *dest)
{
    sprintf(dest, "%02X:%02X:%02X:%02X:%02X:%02X", frame[0], frame[1], frame[2], frame[3], frame[4], frame[5]);
}

void getSrc(const unsigned char frame[], char *dest)
{
    sprintf(dest, "%02X:%02X:%02X:%02X:%02X:%02X", frame[6], frame[7], frame[8], frame[9], frame[10], frame[11]);
}

uint16_t getEtherType(const unsigned char frame[])
{
    __uint16_t result = (frame[12] << 8) | frame[13];
    return result;
}

const char *getMacType(const unsigned char frame[])
{
    if (memcmp(frame, "\xFF\xFF\xFF\xFF\xFF\xFF", 6) == 0)
        return "(Broadcast)\n";
    return (frame[0] & 0x01) ? "(Multicast)\n" : "(Unicast)\n";
}

void parse_frame(const unsigned char frame[], int frame_len)
{

    // printing destination MAC:
    unsigned char dest[18];
    getDest(frame, dest);
    printf("Destination MAC: %s", dest);

    // if (strcmp(dest, "FF:FF:FF:FF:FF:FF") == 0)
    // {
    //     printf("(Broadcast)");
    // }
    // else
    // {

    //     if (frame[0] & 0x01)
    //         printf("(Multicast)");
    //     else
    //         printf("(Unicast)");
    // }
    // printf("\n");
    printf("%s", getMacType(frame));

    // printing source MAC:
    unsigned char src[18];
    getSrc(frame, src);
    printf("Source MAC:      %s", src);

    printf("\n");

    // Ether Type:
    uint16_t etherType = getEtherType(frame);
    printf("EtherType:       0x%04X", etherType);

    switch (etherType)
    {
    case ETHERTYPE_IPV4:
        printf("(IPv4)");
        break;
    case ETHERTYPE_IPV6:
        printf("(IPv6)");
        break;
    case ETHERTYPE_VLAN:
        printf("(VLAN tagged)");
        break;
    case ETHERTYPE_ARP:
        printf("(ARP)");
        break;
    default:
        printf("(Unknown/Custom)");
        break;
    }
    printf("\n");

    // Payload length:
    size_t len = frame_len - ETH_HEADER_LEN;
    printf("Payload length:  %ld\n", len);

    // First 16 or less payload bytes:
    int f_len = (len < 16) ? len : 16;
    printf("Payload:         ");
    for (int i = 0; i < f_len; i++) // starts from 14 because dest,
                                    //  src and type is 14 bytes and next is
                                    // 15th nut indexing is from 0 so 15-1

    {
        printf("%02X ", frame[ETH_HEADER_LEN + i]);
    }
    printf("(First %d bytes)", f_len);

    printf("\n");
}

unsigned char *generateFrame(FILE *fp, int *frame_len)
{
    unsigned char buffer[4096];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer) - 1, fp);
    if (bytesRead >= sizeof(buffer))
    {
        printf("Frame Too large\n");
        exit(1);
    }
    buffer[bytesRead] = '\0';
    int cnt = 0;
    unsigned char *frame = malloc(bytesRead);

    char *token = strtok((char *)buffer, " \n");

    while (token != NULL)
    {
        unsigned char byte = (unsigned char)strtoul(token, NULL, 16);
        // printf("%02X ", byte);
        frame[cnt++] = byte;
        token = strtok(NULL, " \n");
    }
    // printf("Count is: %d.", cnt);
    *frame_len = cnt;
    return frame;
}

void print_help(const char *progname)
{
    printf("Usage:\n");
    printf("  %s                # Use hardcoded test frame\n", progname);
    printf("  %s <file>         # Read hex dump from a file\n", progname);
    printf("  %s <hex bytes...> # Parse hex bytes given directly on command line\n", progname);
    printf("  %s -h             # Show help message\n", progname);
    printf("\nExamples:\n");
    printf("  %s frame.txt\n", progname);
    printf("  %s 00 1A 2B 3C 4D 5E 00 0C 29 AA BB CC 08 00\n", progname);
}

int main(int argc, char *argv[])
{
    printf("Number of arguments: %d", argc);

    printf("\n");
    //**********************ARP PACKET**********************
    // unsigned char frame[] = {
    //     0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination MAC (Broadcast)
    //     0x08, 0x00, 0x27, 0x13, 0x69, 0xEA, // Source MAC
    //     0x08, 0x06,                         // EtherType (ARP)
    //     0x00, 0x01, 0x08, 0x00, 0x06, 0x04  // Payload start
    // };

    // **********************IPv4 PACKET**********************
    unsigned char frame[] = {
        0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E, // Dest MAC
        0x00, 0x0C, 0x29, 0xAA, 0xBB, 0xCC, // Src MAC
        0x08, 0x00,                         // EtherType: IPv4 (0x0800)
        0x45, 0x00, 0x00, 0x54, 0xA6, 0xF2, 0x40, 0x00,
        0x40, 0x01, 0xB7, 0xE6, 0xC0, 0xA8, 0x00, 0x68};

    // **********************IPv6 PACKET**********************
    // unsigned char frame[] = {
    //     0x33, 0x33, 0x00, 0x00, 0x00, 0x16, // Multicast Dest MAC
    //     0x00, 0x0C, 0x29, 0xAA, 0xBB, 0xCC, // Src MAC
    //     0x86, 0xDD,                         // EtherType: IPv6 (0x86DD)
    //     0x60, 0x00, 0x00, 0x00, 0x00, 0x14, 0x3A, 0xFF};

    // **********************Minimum Ethernet Frame**********************
    // unsigned char frame[] = {
    //     0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, // Dest MAC
    //     0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67, // Src MAC
    //     0x08, 0x00,                         // EtherType: IPv4
    //     0x00                                // Only 1 byte payload
    // };

    // **********************Custom***********************
    // unsigned char frame[] = {
    //     0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, // Dest MAC
    //     0x11, 0x22, 0x33, 0x44, 0x55, 0x66, // Src MAC
    //     0x88, 0xB5,                         // EtherType: Custom
    //     0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE};

    if (argc == 1)
    {
        // this is hardcoded approach
        printf("Hardcoded\n\n");
        parse_frame(frame, sizeof(frame)); // directly call the fn
        return 0;
    }
    if (argc >= 2)
    {
        if (argc == 2 && strcmp(argv[1], "-h") == 0)
        {
            print_help(argv[0]);
            return 0;
        }
        // Either file i/p or a hex dump.
        // check if the file opens successfully
        FILE *fp = fopen(argv[1], "r");
        if (fp != NULL)
        {
            // int frame_len;
            // unsigned char *file_frame = generateFrame(fp, &file_frame);
            // parse_frame(file_frame, frame_len);
            // // now we create the byte array.
            // fclose(fp);
            // free(file_frame);
            int frame_len;
            unsigned char *file_frame = generateFrame(fp, &frame_len);
            parse_frame(file_frame, frame_len);
            fclose(fp);
            free(file_frame);
        }
        else if (argc >= ETH_HEADER_LEN)
        {
            // header has dest addr(6 bytes), src addr(6 bytes), etherType(2 bytes).
            printf("Hex dump\n");
            // now we create the byte array.
            int frame_len = 0;
            unsigned char *frame = malloc(argc - 1);
            for (int i = 1; i < argc; i++)
            {
                frame[frame_len++] = (unsigned char)strtoul(argv[i], NULL, 16);
            }
            parse_frame(frame, frame_len);
            free(frame);
        }
        else
        {
            printf("Invalid input\n");
            return 0;
        }
    }

    return 0;
}