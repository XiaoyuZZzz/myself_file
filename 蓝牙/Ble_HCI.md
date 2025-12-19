# 低功耗蓝牙协议栈学习之HCI

> 自我阐述

这几天的学习来看，蓝牙协议栈核心在于“协议栈”，就是很多协议像栈一样层层递进。而本文主要阐述HCI层。

本文主要简单的总结，什么是HCI、HCI流控、HCI的协议格式。

最后举一个BLE 广播的例子来阐述一下HCI的自己封装



## 什么是HCI？

首先来看看蓝牙5.0核心手册是怎么说的

```
The HCI provides a uniform command method of accessing Controller 
capabilities. The HCI Link commands provide the Host with the ability to control 
connections to other BR/EDR Controllers. For the BR/EDR Controller, these 
commands typically involve the Link Manager (LM) to exchange LMP 
commands or the Link Layer (LL) to exchange LL Control packets with remote 
Bluetooth devices. For details see [Vol 2] Part C, Link Manager Protocol 
Specification and [Vol 6] Part B, Link Layer Specification. For AMP Controllers, 
these commands typically involve the AMP PAL. For details see the 
appropriate PAL specification (see [Vol 5] Part A, 802.11 Protocol Adaptation 
Layer Functional Specification). 

The HCI Policy commands are used to affect the behavior of the local and 
remote LM or LL. These Policy commands provide the Host with methods of 
influencing how the LM or LL manages the piconet. The Controller & 
Baseband, Informational, and Status commands provide the Host access to 
various registers in the Controller.

HCI commands may take different amounts of time to be completed. Therefore, 
the results of commands will be reported back to the Host in the form of an 
event. For example, for most HCI commands the Controller will generate the 
Command Complete event when a command is completed. This event 
contains the return parameters for the completed HCI command. For enabling 
the Host to detect errors on the HCI-Transport Layer, there needs to be a 
timeout between the transmission of the Host’s command and the reception of 
the Controller’s response (e.g. a Command Complete or Command Status 
event). Since the maximum response timeout is strongly dependent on the 
HCI-Transport Layer used, it is recommended to use a default value of one 
second for this timer. This amount of time is also dependent on the number of 
commands unprocessed in the command queue.
```

**简而言之就是Host层与Controller层之间的接口，两个层级之间通过HCI进行交互。而蓝牙组织就规定了数据的格式和内容**





## HCI流控？

![ChatGPT Image 2025年10月31日 00_15_03](C:\Users\user\Downloads\ChatGPT Image 2025年10月31日 00_15_03.png)



### 1. HOST TO CONTROLLER DATA FLOW CONTROL

**Host to Controller Data Flow Control** 是指在蓝牙通信中，**主机（Host）** 和 **控制器（Controller）** 之间的数据流量管理机制。它的目的是确保主机和控制器之间的数据传输顺畅，避免数据丢失、过载或者冲突。通过这种机制，主机和控制器可以协调彼此的发送和接收速率，确保通信链路的稳定性。

在蓝牙通信中，尤其是 **HCI（Host Controller Interface）** 层，主机和控制器之间的通信通常是基于**命令和事件的交互**，并且涉及数据包的传输。数据流控制（Flow Control）是管理这些数据包传输的机制，它保证了两者之间的数据传输能够有效且不丢失。

在 HCI 协议中，数据流控制主要体现在以下几个方面：

1. **HCI Command Complete Event**：
   - 当控制器完成命令的处理时，它会通过事件包通知主机。如果控制器有更多的任务需要处理，流控机制确保它能够以合适的速率响应。
2. **HCI Data Packets**：
   - 这些是用于传输数据的包。通过流控机制，主机和控制器能够协商何时开始或暂停数据包的传输。
3. **Queue Management**：
   - 主机和控制器可能有各自的队列，用来存储待处理的命令或数据。数据流控制确保队列不会溢出，并根据缓冲区的容量调整发送速率。

### 2. CONTROLLER TO HOST DATA FLOW CONTROL

**Controller to Host Data Flow Control**（控制器到主机的数据流控制）是蓝牙通信中，**控制器（Controller）** 向 **主机（Host）** 提供的流量管理机制。它用于确保主机能够按合理的速率接收从控制器发送的数据，防止主机的缓冲区溢出，确保数据传输的高效和可靠。

在蓝牙通信中，尤其是基于 **HCI（Host Controller Interface）** 的通信中，控制器和主机之间的交互非常关键。控制器需要将其处理的事件、数据或者命令的结果（例如，连接状态、数据包等）发送给主机，而 **Controller to Host Data Flow Control** 就是在此过程中确保数据流不超出主机的接收能力。

在蓝牙 HCI 协议中，数据流控制主要通过以下几种方式来实现：

1. **控制器通过事件通知主机缓冲区状态**：
   - 控制器可以通过发送 **HCI Event**，例如 `HCI_FLOW_CONTROL_EVENT`，通知主机当前的接收状态。如果控制器的缓冲区已满，它可以指示主机暂停或减缓发送数据。
2. **数据包发送和接收的调节**：
   - 控制器发送的数据包（例如 HCI 数据包）需要依据主机的接收能力进行调节。如果主机的接收队列已满，控制器就需要暂停发送，等待主机的缓冲区腾出空间。
3. **队列管理**：
   - 控制器和主机之间通过队列进行数据传输。控制器需要根据队列的可用空间来决定是否继续发送数据。
4. **优先级调度**：
   - 控制器还可以根据数据的优先级来控制数据的发送顺序。例如，优先发送高优先级的命令或事件，而低优先级的数据可能会等待更长时间。

### 3. Host to Controller 

Host to COntroller有两种流控，分别为Packet-based Data Flow Control 和 Data-Block-Based Data Flow Control。他们之间通过**HCI_Write_Flow_Control_Mode**进行切换。（Packet-based data flow control 是芯片默认的）。![image-20251030235636800](C:\Users\user\AppData\Roaming\Typora\typora-user-images\image-20251030235636800.png)



### 4. Controller to Host











**通俗易懂的说，就是host和controller之间通过“硬件”、“软件”，流控对数据进行控制**。

下面是一个命令的流程：

#### 1. **Read_Buffer_Size 命令**：

在初始化阶段，**主机** 会向 **主机控制器** 发送一个 `Read_Buffer_Size` 命令，这个命令的作用是请求主机控制器返回两个关键参数：

- **最大数据包大小**：确定从主机到控制器的 HCI ACL 和 SCO 数据包（数据部分，不包括报头）的最大长度。
- **缓存数量**：表示主机控制器可以缓存的、等待传输的 HCI ACL 和 SCO 数据包的数量。

这些参数帮助主机了解控制器的缓存能力，从而可以决定如何合理地发送数据，避免过多数据导致溢出或阻塞。

#### 2. **Number Of Completed Packets 事件**：

主机控制器通过发送 **Number Of Completed Packets** 事件来告知主机，已成功发送的 HCI 数据包数量。事件中会返回以下信息：

- **连接句柄列表**：哪些连接的 HCI 数据包已经被发送完成。
- **每个连接的已完成数据包数**：对于每个连接句柄，已经完成的数据包数量。

在没有新的事件发生时，这个信息会从连接建立时开始计算。事件会定期发送，帮助主机实时了解已完成的数据包数量。

#### 3. **数据流控制**：

- 每当 **主机** 发送一个数据包，控制器的缓存空间会减少一个数据包的容量。控制器通过 **Number Of Completed Packets** 事件来告诉主机，这些数据包已经完成（包括传输、清除和回送至主机）。
- 主机根据 **Read_Buffer_Size** 返回的参数（控制器可以缓存的总数据包数量）和 **Number Of Completed Packets** 事件的反馈来判断当前缓存的使用情况。

主机通过这些信息来决定是否继续发送数据包。主机会根据 **控制器缓存剩余空间** 和 **完成的数据包数量** 来动态调整发送数据的频率。

#### 4. **SCO 流控制失效的情况**：

如果 **SCO（Synchronous Connection-Oriented）** 数据流控制失效，那么 **Number Of Completed Packets 事件** 就无法在 SCO 连接句柄中报告。这意味着，对于 SCO 连接，主机无法通过这个事件来获得关于 SCO 数据包是否已发送完成的信息。

#### 5. **事件发送频率**：

**Number Of Completed Packets** 事件的发送频率由 **蓝牙控制器厂商**来决定。它通常是根据控制器的缓存和处理能力来调整的，以确保主机可以及时获得关于数据包状态的反馈。





比如说一开始的时候会发送readbuffer的命令，controller会返回SCO和ACL的大小，比如说我总共只能发送16个event，我已经发送了16个，这个时候需要controller返回16个，我才能发送。意思就是说，两者之间进行一个竞争资源。达到限制的数目了的时候，只有controller释放了，host才能发送。



## HCI格式

![image-20251031214042795](C:\Users\user\AppData\Roaming\Typora\typora-user-images\image-20251031214042795.png)

这里就说一下HCI Command packet的分析，主要讲述一下OpCode这个如何进行封装，这个数据是小端存储。OCF占去10bit OGF占区6bit,举一个例子。
OGF的值为3 OCF的值为8
0011  1000，如果按照正常来说，是不是OCF<< 10 | OGF？但是这个是小端存储的方式，那么就相当于 OCF >> 8  | OGF << 2

```C
p_data[0] = ocf && 0xff
p_data[1] = (ocf >> 8) | (ogf << 2)
```





## Ble Adv example

1. 首先低功耗蓝牙需要发送`set event mask`这个的作用可以参考蓝牙核心手册。这个是用来告诉controller需要上报那些事件的。

2. 发送支持ble的command`write le host support`
3. 发送Ble的广播设计`LE Set Adverting Parameters` 
4. 发送Ble的广播设计数据`LE Set Adverting Data`
5. 开启Ble的广播`LE Set Adverting Enable`
6. 收到Controller的回应数据
7. 关闭Ble的广播`LE Set Adverting Enable`



蓝牙5.0手册，page 1245

**HCI_LE_Set_Event_Mask**和**HCI_LE_Read_Buffer_Size**就不过多介绍了，就是上述解释的Read_Buffer_Size的命令。
**HCI_LE_Read_Local_Supported_Features**就是发送支持Ble的command

**HCI_LE_Set_Advertising_Parameters**设置广播的参数

![image-20251031221416077](C:\Users\user\AppData\Roaming\Typora\typora-user-images\image-20251031221416077.png)

**HCI_LE_Set_Advertising_Data** 设置广播包的内容

![image-20251031222354957](C:\Users\user\AppData\Roaming\Typora\typora-user-images\image-20251031222354957.png)

**HCI_LE_Set_Advertising_Enable** 开启或关闭广播数据包

![image-20251031222449718](C:\Users\user\AppData\Roaming\Typora\typora-user-images\image-20251031222449718.png)





Code：下面是一个如何封装设置广播数据的函数

```
//这个函数是一个封装Commad的函数，可以看到HCI格式这一目录我有解释
struct bt_pbuf_t *hci_cmd_ass(struct bt_pbuf_t *p, uint8_t ocf, uint8_t ogf, uint8_t len)
{
    ((uint8_t *)p->payload)[0] = (ocf & 0xff); /* OCF & OGF */
    ((uint8_t *)p->payload)[1] = (ocf >> 8)|(ogf << 2);
    ((uint8_t *)p->payload)[2] = len-HCI_CMD_HDR_LEN; /* Param len = plen - cmd hdr  */
    if(hci_pcb->numcmd != 0)
    {
        --hci_pcb->numcmd; /* Reduce number of cmd packets that the host controller can buffer */
    }
    return p;
}


// 这是封装设置广播包的命令，可以查看蓝牙5.0核心手册1251页，其中有8个参数
//	adv_int_min 这表示最小蓝牙广播间隔，N * 625us 区间为20ms到10.24s
//	adv_int_max	这表示最大蓝牙广播间隔。N * 625us 区间为20ms到10.24s
// 	adv_type		广播类型，具体查看手册
//	own_address_typ 地址类型，具体查看手册
//	peer_address_type 对方地址类型，具体查看手册
// 	peer_address   	对方的地址
//	channel_map		广播的信道
//	filter_policy	广播过滤策略

err_t hci_le_set_adv_param(uint16_t adv_int_min, uint16_t adv_int_max, uint8_t adv_type,
                           uint8_t own_address_typ, uint8_t peer_address_type,struct bd_addr_t *peer_address, uint8_t channel_map, uint8_t filter_policy)
{
    struct bt_pbuf_t *p;
    uint8_t offset = 0;
    if((p = bt_pbuf_alloc(BT_TRANSPORT_TYPE, HCI_SET_LE_ADV_PARAM_PLEN, BT_PBUF_RAM)) == NULL)
    {
        BT_HCI_TRACE_ERROR("ERROR:file[%s],function[%s],line[%d] bt_pbuf_alloc fail\n",__FILE__,__FUNCTION__,__LINE__);
        return BT_ERR_MEM;
    }
    /* Assembling command packet */
    p = hci_cmd_ass(p, HCI_LE_SET_ADV_PARAM, HCI_LE, HCI_SET_LE_ADV_PARAM_PLEN);
    offset += 3;
    bt_le_store_16((uint8_t *)p->payload,offset,adv_int_min);
    offset += 2;
    bt_le_store_16((uint8_t *)p->payload,offset,adv_int_max);
    offset += 2;
    ((uint8_t *)p->payload)[offset] = adv_type;
    offset += 1;
    ((uint8_t *)p->payload)[offset] = own_address_typ;
    offset += 1;
    ((uint8_t *)p->payload)[offset] = peer_address_type;
    offset += 1;
    memcpy(((uint8_t *)p->payload)+offset, peer_address->addr, BD_ADDR_LEN);
    offset += BD_ADDR_LEN;
    ((uint8_t *)p->payload)[offset] = channel_map;
    offset += 1;
    ((uint8_t *)p->payload)[offset] = filter_policy;
    phybusif_output(p, p->tot_len,PHYBUSIF_PACKET_TYPE_CMD);
    bt_pbuf_free(p);

    return BT_ERR_OK;
}
