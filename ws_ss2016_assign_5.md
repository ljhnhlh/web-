# IPSec 传输模式下 ESP 报文的装包与拆包过程

## IP Header 的结构：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218215814429.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MzAzODYy,size_16,color_FFFFFF,t_70)

ESP 协议有隧道模式和传输模式，隧道模式会加密所有的IP packet并添加新的header；

传输模式则仅加密传输的数据，不加密IP Header；

## 装包过程

### 隧道模式

(1) 在原 IP 报文末尾添加 ESP trailer 信息，其中ESP trailer 包含三部分:Padding ,Pad length, Next header。Padding 和 Pad length用于块加密，Next header 用来标明被封装的原报文的协议类型，如：Next header = 4，则为IP类型

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218221318926.png)



(2) 将步骤（1）得到的报文整体进行加密，得到密文M~0~，加密算法由SA给出

(3) 在M~0~ 前面添加 ESP header得到M~1~ 构成认证部分，其中ESP header由 SPI 和Seq# 两部分组成。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218221907105.png)

 

(4) 对M~1~ 进行完整性认证，得到一个32bit的完整性度量值（MAC），将MAC附在ESP报文的尾部

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218222716175.png)



(5)构造新的IP Header附在ESP报文前面组成新的IP报文。个新的 IP header
的 IP 地址由路由器和安全网关解释，可以和原报文不同。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218222909809.png)

下图中，左，右分别为原IP报文和IPSec报文，新IP header 的 proto  = 50，即为ESP 报文

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218223050526.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MzAzODYy,size_16,color_FFFFFF,t_70)



![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218214850716.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MzAzODYy,size_16,color_FFFFFF,t_70)

### 传输模式

传输模式加密除IP Header的所有内容且不添加新的IP Header，其他过程与隧道模式一致

(1) 将除IP header 外的所有数据进行加密，得到密文 M~0~

(2) 将ESP Header 附加到 M~0~ 头部，得到 M~1~

(3)对M~1~ 进行完整性认证，得到一个32bit的完整性度量值（MAC），将MAC附在ESP报文的尾部，得到 M~2~

(4) 修改IP header 的proto 值 为 50，附加到 M~2~ 头部，生成IPSec 包进行传输

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218215408475.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MzAzODYy,size_16,color_FFFFFF,t_70)



## 拆包过程

### 隧道模式

(1) 接收方收到 IP 报文后，发现协议类型是50，表明这是一个 ESP包。首先查看 ESP header，通过 SPI 决定数据报文所对应的 SA，获得对应的模式 (tunnel/transport mode) 以及安全规范。

(2) 计算M~1~ 部分，与末尾的MAC比较以验证数据完整性

(3) 检查 Seq# 里的顺序号，保证数据是最新的，防止重放攻击

(4)根据 SA 所提供的加密算法和密钥，解密被加密过的数据，得到
原 IP 报文与 ESP trailer。

5) 根据 ESP trailer 的填充长度信息，找出填充字段的长度，删去
后得到原来的 IP 报文。

(6) 根据得到的原 IP 报文的目的地址进行转发。



### 传输模式

(1) 接收方收到 IP 报文后，发现协议类型是50，表明这是一个 ESP包。查看 ESP header，通过 SPI 决定数据报文所对应的 SA，获得对应的模式 (tunnel/transport mode) 以及安全规范。

(2) 计算M~1~ 部分，与末尾的MAC比较以验证数据完整性

(3) 检查 Seq# 里的顺序号，保证数据是最新的，防止重放攻击

(4)根据 SA 所提供的加密算法和密钥，解密被加密过的数据，得到
原 IP 数据报文与 ESP trailer。

5) 根据 ESP trailer 的填充长度信息，找出填充字段的长度，删去
后得到原来的 IP数据 报文。

(6) 根据IP 报文的目的地址进行转发。



> ps: 图片上带有水印，是由于截图经过个人博客，由链接生成的。