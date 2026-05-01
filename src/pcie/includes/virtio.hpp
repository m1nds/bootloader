#ifndef VIRTIO_HPP
#define VIRTIO_HPP

// Based on https://docs.oasis-open.org/virtio/virtio/v1.0/cs04/virtio-v1.0-cs04.pdf

#include <pcie.hpp>
#include <cstdint>

/* Custom type definitions */
typedef uint8_t u8;
typedef uint16_t le16;
typedef uint32_t le32;
typedef uint64_t le64;

/* Common configuration */
constexpr int VIRTIO_PCI_CAP_COMMON_CFG = 1;
/* Notifications */
constexpr int VIRTIO_PCI_CAP_NOTIFY_CFG = 2;
/* ISR Status */
constexpr int VIRTIO_PCI_CAP_ISR_CFG = 3;
/* Device specific configuration */
constexpr int VIRTIO_PCI_CAP_DEVICE_CFG = 4;
/* PCI configuration access */
constexpr int VIRTIO_PCI_CAP_PCI_CFG = 5;

constexpr int VIRTIO_CONFIG_S_ACKNOWLEDGE = 1;
constexpr int VIRTIO_CONFIG_S_DRIVER = 2;
constexpr int VIRTIO_CONFIG_S_DRIVER_OK = 4;
constexpr int VIRTIO_CONFIG_S_FEATURES_OK = 8;

constexpr int VIRTIO_NET_F_CSUM = 1 << 0;
constexpr int VIRTIO_NET_F_GUEST_CSUM = 1 << 1;
constexpr int VIRTIO_NET_F_MAC = 1 << 5;
constexpr int VIRTIO_F_VERSION_1 = 1 << 31;

struct virtio_pci_cap {
    u8 cap_vndr; /* Generic PCI field: PCI_CAP_ID_VNDR */
    u8 cap_next; /* Generic PCI field: next ptr. */
    u8 cap_len; /* Generic PCI field: capability length */
    u8 cfg_type; /* Identifies the structure. */
    u8 bar; /* Where to find it. */
    u8 padding[3]; /* Pad to full dword. */
    le32 offset; /* Offset within bar. */
    le32 length; /* Length of the structure, in bytes. */
} __attribute__((packed));

struct virtio_pci_common_cfg {
/* About the whole device. */
    le32 device_feature_select; /* read-write */
    le32 device_feature; /* read-only for driver */
    le32 driver_feature_select; /* read-write */
    le32 driver_feature; /* read-write */
    le16 msix_config; /* read-write */
    le16 num_queues; /* read-only for driver */
    u8 device_status; /* read-write */
    u8 config_generation; /* read-only for driver */
/* About a specific virtqueue. */
    le16 queue_select; /* read-write */
    le16 queue_size; /* read-write, power of 2, or 0. */
    le16 queue_msix_vector; /* read-write */
    le16 queue_enable; /* read-write */
    le16 queue_notify_off; /* read-only for driver */
    le64 queue_desc; /* read-write */
    le64 queue_avail; /* read-write */
    le64 queue_used; /* read-write */
} __attribute__((packed));

struct virtio_pci_notify_cap {
    struct virtio_pci_cap cap;
    le32 notify_off_multiplier; /* Multiplier for queue_notify_off. */
} __attribute__((packed));

class virtio_net : public pci_device {
    public:
        using pci_device::pci_device;

        uint8_t* get_virtio_cap(uint8_t cfg_type);
        struct virtio_pci_common_cfg* config(int virtio_config);
        void setup_virtq();

        static void init_virtio_net();

    private:
        static uint16_t* rx_reg;
        static uint16_t* tx_reg;
        static struct virtq rx __attribute__((aligned(4096)));;
        static struct virtq tx __attribute__((aligned(4096)));;
};

constexpr int VIRTIO_NET_HDR_F_NEEDS_CSUM = 1;
constexpr int VIRTIO_NET_HDR_GSO_NONE = 0;
constexpr int VIRTIO_NET_HDR_GSO_TCPV4 = 1;
constexpr int VIRTIO_NET_HDR_GSO_UDP = 3;
constexpr int VIRTIO_NET_HDR_GSO_TCPV6 = 4;
constexpr int VIRTIO_NET_HDR_GSO_ECN = 0x80;

struct virtio_net_hdr {
    u8 flags;
    u8 gso_type;
    le16 hdr_len;
    le16 gso_size;
    le16 csum_start;
    le16 csum_offset;
    le16 num_buffers;
};

#endif /* VIRTIO_HPP */
