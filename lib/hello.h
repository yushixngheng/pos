#ifndef HELLO_H
#define HELLO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void HelloFunc();




//标准数据池字典
#define POSP_DATAPOOL_MSG_TYPE					0				//消息类型
#define POSP_DATAPOOL_BITMAP 					1				//位图
#define POSP_DATAPOOL_CARD_CODE					2				//交易卡号
#define POSP_DATAPOOL_PROC_CODE					3				//交易处理码
#define POSP_DATAPOOL_TRADE_AMT					4				//交易金额
#define POSP_DATAPOOL_SETTLE_AMT				5				//清算金额
#define POSP_DATAPOOL_TRANS_TIME				7				//交易传输时间
#define POSP_DATAPOOL_TERM_WATER				11				//POS交易流水号
#define POSP_DATAPOOL_SYS_TIME					12				//POS交易时间
#define POSP_DATAPOOL_SYS_DATE					13				//POS交易日期
#define POSP_DATAPOOL_CARD_PERIOD				14				//卡有效期
#define POSP_DATAPOOL_SETTLE_DATE				15				//清算日期
#define POSP_DATAPOOL_MERCHANT_MCC				18				//商户MCC
#define POSP_DATAPOOL_INPUT_METHOD				22				//服务点输入方式
#define POSP_DATAPOOL_CARD_SEQNO				23				//卡序列号
#define POSP_DATAPOOL_SERVICE_CODE				25				//服务点交易码
#define POSP_DATAPOOL_ACQCENTER_CODE			32				//受理机构中心代码
#define POSP_DATAPOOL_FWDCENTER_CODE			33				//发送机构标识码
#define POSP_DATAPOOL_TRACK2					35				//2磁道信息
#define POSP_DATAPOOL_TRACK3					36				//3磁道信息
#define POSP_DATAPOOL_REF_CODE					37				//检索参考号
#define POSP_DATAPOOL_AUTH_CODE					38				//授权码
#define POSP_DATAPOOL_RESP_CODE					39				//39域返回码
#define POSP_DATAPOOL_TERMINAL_CODE				41				//终端号
#define POSP_DATAPOOL_MERCHANT_CODE				42				//商户号
#define POSP_DATAPOOL_MERCHANT_NAME				43				//受卡方名称地址
#define POSP_DATAPOOL_CURRENCY_CODE				49				//货币代码
#define POSP_DATAPOOL_PIN 						52				//个人识别码(银行密码) PIN BLOCK
#define POSP_DATAPOOL_SECURITY_INFO				53				//安全信息域
#define POSP_DATAPOOL_ADDITION_AMT				54				//实际金额域(余额)
#define POSP_DATAPOOL_FRI55						55				//IC卡数据域
#define POSP_DATAPOOL_FRI62						62				//密钥数据域
#define POSP_DATAPOOL_MAC 						64          	//报文鉴别码
#define POSP_DATAPOOL_ORI_DATA 					90				//原始数据元
#define POSP_DATAPOOL_CUP_RESVERD				122				//CUPS保留域，用于匹配原始交易信息
#define POSP_DATAPOOL_MAC128					128				//128域的报文鉴别码

//专门字段
#define POSP_DATAPOOL_ORI_SERVICE_CODE			129				//原交易SERCICE CODE
#define POSP_DATAPOOL_SYS_YEAR					130				//交易年份
#define POSP_DATAPOOL_CHANNEL_STATUS			131				//通道通信状态
#define POSP_DATAPOOL_TPDU						133				//tpdu
#define POSP_DATAPOOL_MSG_BODY					134				//报文主体
#define POSP_DATAPOOL_PACKET_HEADER				135				//报文头
#define POSP_DATAPOOL_MSGTYPE_CODE				136				//POS 60.1消息类型码
#define POSP_DATAPOOL_BATCH_CODE  				137				//批次号
#define POSP_DATAPOOL_NET_CODE  				138				//网络管理信息码
#define POSP_DATAPOOL_POS_INPUT_ABILITY			139				//POS 60.4终端读取能力
#define POSP_DATAPOOL_PBOC_CONDITION_CODE		140				//POS 60.5基于PBOC借贷记IC卡条件代码
#define POSP_DATAPOOL_PART_PAY_FLAG				141				//部分扣款条件标志
#define POSP_DATAPOOL_ACCOUNT_TYPE				142             //账户类型
#define POSP_DATAPOOL_ORI_BATCH_CODE			143				//原交易批次号
#define POSP_DATAPOOL_ORI_TERM_WATER			144				//原交易终端流水号
#define POSP_DATAPOOL_ORI_SYS_DATE				145				//原交易日期
#define POSP_DATAPOOL_PROTOCOL_CODE				148				//协议编号
#define POSP_DATAPOOL_CHANNEL_CODE				149				//通道编号
#define POSP_DATAPOOL_TRADE_TYPE				151				//交易类型
#define POSP_DATAPOOL_CHANNEL_TERMINAL_CODE		154				//通道终端号
#define POSP_DATAPOOL_CHANNEL_MERCHANT_CODE		156				//通道商户号
#define POSP_DATAPOOL_CHANNEL_TIME				157				//通道返回时间
#define POSP_DATAPOOL_CHANNEL_DATE				158				//通道返回日期
#define POSP_DATAPOOL_CHANNEL_TERM_WATER		159				//通道流水号
#define POSP_DATAPOOL_CHANNEL_BATCH_CODE		160				//通道批次号
#define POSP_DATAPOOL_CHANNEL_TPDU				161				//通道TPDU
#define POSP_DATAPOOL_TRADE_ATTR				162				//交易属性
#define POSP_DATAPOOL_SYS_CODE					163				//系统流水唯一号
#define POSP_DATAPOOL_TRADE_STATUS				165				//交易状态
#define POSP_DATAPOOL_FRI55_LEN					167				//55域长度
#define POSP_DATAPOOL_ORI_CHANNEL_BATCH_CODE	168				//原交易通道批次号
#define POSP_DATAPOOL_ORI_CHANNEL_TERM_WATER	169				//原交易通道流水号
#define POSP_DATAPOOL_ORI_CHANNEL_DATE			170				//原交易通道日期
#define POSP_DATAPOOL_ORI_TRADE_AMT				171				//原交易金额
#define POSP_DATAPOOL_ORI_CARD_CODE				172				//原交易卡号
#define POSP_DATAPOOL_ORI_SYS_CODE				173				//原交易系统流水唯一号
#define POSP_DATAPOOL_ORI_CHANNEL_STATUS		174				//原交易通道通信状态
#define POSP_DATAPOOL_ORI_TRADE_STATUS			175  			//原交易状态
#define POSP_DATAPOOL_ORI_TRADE_TYPE			176				//原交易类型
#define POSP_DATAPOOL_ORI_RESP_CODE				177				//原交易39域返回码
#define POSP_DATAPOOL_ORI_SETTLE_DATE 			178				//原交易清算日期
#define POSP_DATAPOOL_CHANNEL_PIN				179				//转换后的通道PIN BLOCK
#define POSP_DATAPOOL_TPK						180				//内部终端TPK
#define POSP_DATAPOOL_CHANNEL_AUTH_CODE			181				//通道授权码
#define POSP_DATAPOOL_ORI_REF_CODE				182				//原交易检索参考号


#define POSP_DATAPOOL_IC_DATA					183				//IC卡数据(ASCII编码)
#define POSP_DATAPOOL_ICDATA_LEN				184				//IC卡数据长度
#define POSP_DATAPOOL_AGENT_FEE_COST			185				//代理商成本费率

#define POSP_DATAPOOL_CUP_FIELD60				186				//CUPS机构报文60域
#define POSP_DATAPOOL_CARDHOLDER_AUTH_INFO		187				//CUPS机构报文61域(持卡人验证信息)

#define POSP_DATAPOOL_CUP_HEADER_LEN			188				//CUPS报文头长度
#define POSP_DATAPOOL_CUP_HEADER_VER			189				//CUPS头标识和版本号
#define POSP_DATAPOOL_CUP_HEADER_TOTAL_LEN		190				//CUPS报文总长度
#define POSP_DATAPOOL_CUP_HEADER_DESTID			191				//CUPS报文头目的ID
#define POSP_DATAPOOL_CUP_HEADER_SOURCEID		192				//CUPS报文头源ID
#define POSP_DATAPOOL_CUP_HEADER_RESV			193				//CUPS报文头保留字段域
#define POSP_DATAPOOL_CUP_HEADER_BATCH_CODE		194				//CUPS报文头批次号域
#define POSP_DATAPOOL_CUP_HEADER_TRADE_INFO		195				//CUPS报文头交易信息
#define POSP_DATAPOOL_CUP_HEADER_USER_INFO		196				//CUPS报文头用户信息
#define POSP_DATAPOOL_CUP_HEADER_REJECT_CODE	197				//CUPS报文头拒绝码


#define POSP_DATAPOOL_ORI_MSG_TYPE				201				//原始交易消息类型
#define POSP_DATAPOOL_ORI_CHANNEL_TIME			202				//交易通道交易时间

#define POSP_DATAPOOL_TAK 						203				//内部终端MAC密钥
#define POSP_DATAPOOL_ZPK						204
#define POSP_DATAPOOL_ZAK						205

#define POSP_DATAPOOL_TRADE_BEGIN_TIME			206				//允许交易开始时间
#define POSP_DATAPOOL_TRADE_END_TIME			207				//允许交易结束时间
#define POSP_DATAPOOL_MAX_AMT_PERCARD			208				//每卡每日交易限额
#define POSP_DATAPOOL_MAX_AMT_PERTRADE			209				//每笔交易限额
#define POSP_DATAPOOL_CURRENT_BATCH_CODE		210				//当前内部终端批次号

//成本、费率相关字典
#define POSP_DATAPOOL_AGENT_GRADE_TYPE			211				//代理商成本档位类型
#define POSP_DATAPOOL_AGENT_FEE_TYPE			212				//代理商成本费率类型
#define POSP_DATAPOOL_AGENT_FEE_CREDIT			213				//代理商贷记卡成本费率
#define POSP_DATAPOOL_AGENT_FEE_DEBIT			214				//代理商借记卡成本费率
#define POSP_DATAPOOL_AGENT_FEE_CREDIT_UP		215				//代理商贷记卡成本封顶费率
#define POSP_DATAPOOL_AGENT_FEE_DEBIT_UP		216				//代理商借记卡成本封顶费率
#define POSP_DATAPOOL_AGENT_FEE_CREDIT_FIXED	217				//代理商贷记卡固定费率
#define POSP_DATAPOOL_AGENT_FEE_DEBIT_FIXED		218				//代理商借记卡固定费率

#define POSP_DATAPOOL_CHANNEL_GRADE_TYPE		219 			//通道成本档位	类型
#define POSP_DATAPOOL_CHANNEL_FEE_TYPE			220				//通道成本费率类型
#define POSP_DATAPOOL_CHANNEL_FEE_CREDIT		221				//通道贷记卡成本费率
#define POSP_DATAPOOL_CHANNEL_FEE_DEBIT			222				//通道借记卡成本费率
#define POSP_DATAPOOL_CHANNEL_FEE_CREDIT_UP		223				//通道贷记卡成本封顶费率
#define POSP_DATAPOOL_CHANNEL_FEE_DEBIT_UP		224				//通道借记卡成本封顶费率
#define POSP_DATAPOOL_CHANNEL_FEE_CREDIT_FIXED	225				//通道贷记卡固定费率
#define POSP_DATAPOOL_CHANNEL_FEE_DEBIT_FIXED	226				//通道借记卡固定费率

#define POSP_DATAPOOL_MHT_FEE_TYPE				227				//商户成本费率类型
#define POSP_DATAPOOL_MHT_FEE_CREDIT			228				//商户贷记卡成本费率
#define POSP_DATAPOOL_MHT_FEE_DEBIT				229				//商户借记卡成本费率
#define POSP_DATAPOOL_MHT_FEE_CREDIT_UP			230				//商户贷记卡成本封顶费率
#define POSP_DATAPOOL_MHT_FEE_DEBIT_UP			231				//商户借记卡成本封顶费率
#define POSP_DATAPOOL_MHT_FEE_CREDIT_FIXED		232				//商户贷记卡固定费率
#define POSP_DATAPOOL_MHT_FEE_DEBIT_FIXED		233				//商户借记卡固定费率
#define POSP_DATAPOOL_AMT_SUM_PERCARD			234				//当卡当日交易累计金额

#define POSP_DATAPOOL_AGENT_CODE				235				//代理商编号

#define POSP_DATAPOOL_LOGIN_FLAG				236				//签到标志

#define	POSP_DATAPOOL_LOG_NAME                  237				//日志名
#define	POSP_DATAPOOL_LOG_BUF                   238				//日志内容
#define	POSP_DATAPOOL_LOG_MAXSIZE               239				//日志最大内存

#define POSP_DATAPOOL_CUP_SEND_PACKET			253				//CUPS发送报文
#define POSP_DATAPOOL_CUP_RECV_PACKET			254				//CUPS接收报文


#define POSP_DATAPOOL_POSP_SIZE		(255)





#endif
