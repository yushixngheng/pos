#ifndef _NGX_DICT_H_
#define _NGX_DICT_H_


//标准数据池字典
#define NGX_DATAPOOL_MSG_TYPE					0				//消息类型
#define NGX_DATAPOOL_BITMAP 					1				//位图
#define NGX_DATAPOOL_CARD_CODE					2				//交易卡号
#define NGX_DATAPOOL_PROC_CODE					3				//交易处理码
#define NGX_DATAPOOL_TRADE_AMT					4				//交易金额
#define NGX_DATAPOOL_SETTLE_AMT					5				//清算金额
#define NGX_DATAPOOL_TRANS_TIME					7				//交易传输时间
#define NGX_DATAPOOL_TERM_WATER					11				//POS交易流水号
#define NGX_DATAPOOL_SYS_TIME					12				//POS交易时间
#define NGX_DATAPOOL_SYS_DATE					13				//POS交易日期
#define NGX_DATAPOOL_CARD_PERIOD				14				//卡有效期
#define NGX_DATAPOOL_SETTLE_DATE				15				//清算日期
#define NGX_DATAPOOL_MERCHANT_MCC				18				//商户MCC
#define NGX_DATAPOOL_INPUT_METHOD				22				//服务点输入方式
#define NGX_DATAPOOL_CARD_SEQNO					23				//卡序列号
#define NGX_DATAPOOL_SERVICE_CODE				25				//服务点交易码
#define NGX_DATAPOOL_PIN_MAX_LEN				26				//服务点允许个人密码明文的最大长度
#define NGX_DATAPOOL_ACQCENTER_CODE				32				//受理机构中心代码
#define NGX_DATAPOOL_FWDCENTER_CODE				33				//发送机构标识码
#define NGX_DATAPOOL_TRACK2						35				//2磁道信息
#define NGX_DATAPOOL_TRACK3						36				//3磁道信息
#define NGX_DATAPOOL_REF_CODE					37				//检索参考号
#define NGX_DATAPOOL_AUTH_CODE					38				//授权码
#define NGX_DATAPOOL_RESP_CODE					39				//39域返回码
#define NGX_DATAPOOL_TERMINAL_CODE				41				//终端号
#define NGX_DATAPOOL_MERCHANT_CODE				42				//商户号
#define NGX_DATAPOOL_MERCHANT_NAME				43				//受卡方名称地址
#define NGX_DATAPOOL_CURRENCY_CODE				49				//货币代码
#define NGX_DATAPOOL_PIN 						52				//个人识别码(银行密码) PIN BLOCK
#define NGX_DATAPOOL_SECURITY_INFO				53				//安全信息域
#define NGX_DATAPOOL_ADDITION_AMT				54				//实际金额域(余额)
#define NGX_DATAPOOL_FRI55						55				//IC卡数据域
#define NGX_DATAPOOL_SALES_SLIP					59				//签购单数据域
#define NGX_DATAPOOL_FRI60						60				//自定义数据域
#define NGX_DATAPOOL_ORI_DATA 					61				//原始数据元
#define NGX_DATAPOOL_FRI62						62				//密钥数据域
#define NGX_DATAPOOL_FRI63						63				//操作员数据域
#define NGX_DATAPOOL_MAC 						64          	//报文鉴别码
#define NGX_DATAPOOL_CUP_RESVERD				122				//CUPS保留域，用于匹配原始交易信息
#define NGX_DATAPOOL_MAC128						128				//128域的报文鉴别码

//专门字段
#define NGX_DATAPOOL_ORI_SERVICE_CODE			129				//原交易SERCICE CODE
#define NGX_DATAPOOL_SYS_YEAR					130				//交易年份
#define NGX_DATAPOOL_CHANNEL_STATUS				131				//通道通信状态
#define NGX_DATAPOOL_TPDU						133				//tpdu
#define NGX_DATAPOOL_MSG_BODY					134				//报文主体
#define NGX_DATAPOOL_PACKET_HEADER				135				//报文头
#define NGX_DATAPOOL_MSGTYPE_CODE				136				//POS 60.1消息类型码
#define NGX_DATAPOOL_BATCH_CODE  				137				//批次号
#define NGX_DATAPOOL_NET_CODE  					138				//网络管理信息码
#define NGX_DATAPOOL_POS_INPUT_ABILITY			139				//POS 60.4终端读取能力
#define NGX_DATAPOOL_PBOC_CONDITION_CODE		140				//POS 60.5基于PBOC借贷记IC卡条件代码
#define NGX_DATAPOOL_PART_PAY_FLAG				141				//部分扣款条件标志
#define NGX_DATAPOOL_ACCOUNT_TYPE				142             //账户类型
#define NGX_DATAPOOL_ORI_BATCH_CODE				143				//原交易批次号
#define NGX_DATAPOOL_ORI_TERM_WATER				144				//原交易终端流水号
#define NGX_DATAPOOL_ORI_SYS_DATE				145				//原交易日期
#define NGX_DATAPOOL_PROTOCOL_CODE				148				//协议编号
#define NGX_DATAPOOL_CHANNEL_CODE				149				//通道编号
#define NGX_DATAPOOL_TRADE_TYPE					151				//交易类型
#define NGX_DATAPOOL_CHANNEL_TERMINAL_CODE		154				//通道终端号
#define NGX_DATAPOOL_CHANNEL_MERCHANT_CODE		156				//通道商户号
#define NGX_DATAPOOL_CHANNEL_TIME				157				//通道返回时间
#define NGX_DATAPOOL_CHANNEL_DATE				158				//通道返回日期
#define NGX_DATAPOOL_CHANNEL_TERM_WATER			159				//通道流水号
#define NGX_DATAPOOL_CHANNEL_BATCH_CODE			160				//通道批次号
#define NGX_DATAPOOL_CHANNEL_TPDU				161				//通道TPDU
#define NGX_DATAPOOL_TRADE_ATTR					162				//交易属性
#define NGX_DATAPOOL_SYS_CODE					163				//系统流水唯一号
#define NGX_DATAPOOL_TRADE_STATUS				165				//交易状态
#define NGX_DATAPOOL_FRI55_LEN					167				//55域长度
#define NGX_DATAPOOL_ORI_CHANNEL_BATCH_CODE		168				//原交易通道批次号
#define NGX_DATAPOOL_ORI_CHANNEL_TERM_WATER		169				//原交易通道流水号
#define NGX_DATAPOOL_ORI_CHANNEL_DATE			170				//原交易通道日期
#define NGX_DATAPOOL_ORI_TRADE_AMT				171				//原交易金额
#define NGX_DATAPOOL_ORI_CARD_CODE				172				//原交易卡号
#define NGX_DATAPOOL_ORI_SYS_CODE				173				//原交易系统流水唯一号
#define NGX_DATAPOOL_ORI_CHANNEL_STATUS			174				//原交易通道通信状态
#define NGX_DATAPOOL_ORI_TRADE_STATUS			175  			//原交易状态
#define NGX_DATAPOOL_ORI_TRADE_TYPE				176				//原交易类型
#define NGX_DATAPOOL_ORI_RESP_CODE				177				//原交易39域返回码
#define NGX_DATAPOOL_ORI_SETTLE_DATE 			178				//原交易清算日期
#define NGX_DATAPOOL_CHANNEL_PIN				179				//转换后的通道PIN BLOCK
#define NGX_DATAPOOL_TPK						180				//内部终端TPK
#define NGX_DATAPOOL_CHANNEL_AUTH_CODE			181				//通道授权码
#define NGX_DATAPOOL_ORI_REF_CODE				182				//原交易检索参考号


#define NGX_DATAPOOL_IC_DATA					183				//IC卡数据(ASCII编码)
#define NGX_DATAPOOL_ICDATA_LEN					184				//IC卡数据长度
#define NGX_DATAPOOL_AGENT_FEE_COST				185				//代理商成本费率

#define NGX_DATAPOOL_CUP_FIELD60				186				//CUPS机构报文60域
#define NGX_DATAPOOL_CARDHOLDER_AUTH_INFO		187				//CUPS机构报文61域(持卡人验证信息)

#define NGX_DATAPOOL_CUP_HEADER_LEN				188				//CUPS报文头长度
#define NGX_DATAPOOL_CUP_HEADER_VER				189				//CUPS头标识和版本号
#define NGX_DATAPOOL_CUP_HEADER_TOTAL_LEN		190				//CUPS报文总长度
#define NGX_DATAPOOL_CUP_HEADER_DESTID			191				//CUPS报文头目的ID
#define NGX_DATAPOOL_CUP_HEADER_SOURCEID		192				//CUPS报文头源ID
#define NGX_DATAPOOL_CUP_HEADER_RESV			193				//CUPS报文头保留字段域
#define MGX_DATAPOOL_CUP_HEADER_BATCH_CODE		194				//CUPS报文头批次号域
#define NGX_DATAPOOL_CUP_HEADER_TRADE_INFO		195				//CUPS报文头交易信息
#define NGX_DATAPOOL_CUP_HEADER_USER_INFO		196				//CUPS报文头用户信息
#define NGX_DATAPOOL_CUP_HEADER_REJECT_CODE		197				//CUPS报文头拒绝码


#define NGX_DATAPOOL_ORI_MSG_TYPE				201				//原始交易消息类型
#define NGX_DATAPOOL_ORI_CHANNEL_TIME			202				//交易通道交易时间

#define NGX_DATAPOOL_TAK 						203				//内部终端MAC密钥
#define NGX_DATAPOOL_ZPK						204
#define NGX_DATAPOOL_ZAK						205

#define NGX_DATAPOOL_TRADE_BEGIN_TIME			206				//允许交易开始时间
#define NGX_DATAPOOL_TRADE_END_TIME				207				//允许交易结束时间
#define NGX_DATAPOOL_MAX_AMT_PERCARD			208				//每卡每日交易限额
#define NGX_DATAPOOL_MAX_AMT_PERTRADE			209				//每笔交易限额
#define NGX_DATAPOOL_CURRENT_BATCH_CODE			210				//当前内部终端批次号

//成本、费率相关字典
#define NGX_DATAPOOL_AGENT_GRADE_TYPE			211				//代理商成本档位类型
#define NGX_DATAPOOL_AGENT_FEE_TYPE				212				//代理商成本费率类型
#define NGX_DATAPOOL_AGENT_FEE_CREDIT			213				//代理商贷记卡成本费率
#define NGX_DATAPOOL_AGENT_FEE_DEBIT			214				//代理商借记卡成本费率
#define NGX_DATAPOOL_AGENT_FEE_CREDIT_UP		215				//代理商贷记卡成本封顶费率
#define NGX_DATAPOOL_AGENT_FEE_DEBIT_UP			216				//代理商借记卡成本封顶费率
#define NGX_DATAPOOL_AGENT_FEE_CREDIT_FIXED		217				//代理商贷记卡固定费率
#define NGX_DATAPOOL_AGENT_FEE_DEBIT_FIXED		218				//代理商借记卡固定费率

#define NGX_DATAPOOL_CHANNEL_GRADE_TYPE			219 			//通道成本档位	类型
#define NGX_DATAPOOL_CHANNEL_FEE_TYPE			220				//通道成本费率类型
#define NGX_DATAPOOL_CHANNEL_FEE_CREDIT			221				//通道贷记卡成本费率
#define NGX_DATAPOOL_CHANNEL_FEE_DEBIT			222				//通道借记卡成本费率
#define NGX_DATAPOOL_CHANNEL_FEE_CREDIT_UP		223				//通道贷记卡成本封顶费率
#define NGX_DATAPOOL_CHANNEL_FEE_DEBIT_UP		224				//通道借记卡成本封顶费率
#define NGX_DATAPOOL_CHANNEL_FEE_CREDIT_FIXED	225				//通道贷记卡固定费率
#define NGX_DATAPOOL_CHANNEL_FEE_DEBIT_FIXED	226				//通道借记卡固定费率

#define NGX_DATAPOOL_MHT_FEE_TYPE				227				//商户成本费率类型
#define NGX_DATAPOOL_MHT_FEE_CREDIT				228				//商户贷记卡成本费率
#define NGX_DATAPOOL_MHT_FEE_DEBIT				229				//商户借记卡成本费率
#define NGX_DATAPOOL_MHT_FEE_CREDIT_UP			230				//商户贷记卡成本封顶费率
#define NGX_DATAPOOL_MHT_FEE_DEBIT_UP			231				//商户借记卡成本封顶费率
#define NGX_DATAPOOL_MHT_FEE_CREDIT_FIXED		232				//商户贷记卡固定费率
#define NGX_DATAPOOL_MHT_FEE_DEBIT_FIXED		233				//商户借记卡固定费率
#define NGX_DATAPOOL_AMT_SUM_PERCARD			234				//当卡当日交易累计金额

#define NGX_DATAPOOL_AGENT_CODE					235				//代理商编号

#define NGX_DATAPOOL_LOGIN_FLAG					236				//签到标志

#define	NGX_DATAPOOL_LOG_NAME       	        237				//日志名
#define	NGX_DATAPOOL_LOG_BUF                   	238				//日志内容
#define	NGX_DATAPOOL_LOG_MAXSIZE               	239				//日志最大内存

#define NGX_DATAPOOL_CUP_SEND_PACKET			253				//CUPS发送报文
#define NGX_DATAPOOL_CUP_RECV_PACKET			254				//CUPS接收报文


//256-287 保留各自业务平台使用

#define NGX_DATAPOOL_BUSI_TYPE					288
#define NGX_DATAPOOL_BUSI_REQ_NO				289


//291 app平台占用

#define NGX_DATAPOOL_WATER_TYPE					292			//流水类型
#define NGX_DATAPOOL_V_ACCOUNT					293			//虚拟账号
#define NGX_DATAPOOL_PARTNER_MARKET_PROPORTION 295			//合作方营销费用分润比
#define NGX_DATAPOOL_AGENT_MARKET_PROPORTION	296			//代理商营销费用分润比

#define NGX_DATAPOOL_T0_MHT_FEE_TYPE			297			//t+0商户成本费率类型
#define NGX_DATAPOOL_T0_MHT_FEE_CREDIT			298			//t+0商户贷记卡成本费率
#define NGX_DATAPOOL_T0_MHT_FEE_DEBIT			299			//t+0商户借记卡成本费率
#define NGX_DATAPOOL_T0_MHT_FEE_CREDIT_UP		300			//t+0商户贷记卡成本封顶费率
#define NGX_DATAPOOL_T0_MHT_FEE_DEBIT_UP		301			//t+0商户借记卡成本封顶费率
#define NGX_DATAPOOL_T0_MHT_FEE_CREDIT_FIXED	302			//t+0商户贷记卡固定费率
#define NGX_DATAPOOL_T0_MHT_FEE_DEBIT_FIXED	303			//t+0商户借记卡固定费率


#define NGX_DATAPOOL_T0_AGENT_GRADE_TYPE		304			//代理商成本档位类型
#define NGX_DATAPOOL_T0_AGENT_FEE_TYPE			305			//代理商成本费率类型
#define NGX_DATAPOOL_T0_AGENT_FEE_CREDIT		306			//代理商贷记卡成本费率
#define NGX_DATAPOOL_T0_AGENT_FEE_DEBIT			307			//代理商借记卡成本费率
#define NGX_DATAPOOL_T0_AGENT_FEE_CREDIT_UP		308			//代理商贷记卡成本封顶费率
#define NGX_DATAPOOL_T0_AGENT_FEE_DEBIT_UP		309			//代理商借记卡成本封顶费率
#define NGX_DATAPOOL_T0_AGENT_FEE_CREDIT_FIXED	310			//代理商贷记卡固定费率
#define NGX_DATAPOOL_T0_AGENT_FEE_DEBIT_FIXED	311			//代理商借记卡固定费率

#define NGX_DATAPOOL_OPEN_T0					312			//是否开通T0业务
#define NGX_DATAPOOL_CLEARINGBYSELF				313			//是否开通自主清算
#define NGX_DATAPOOL_EXT_MHT_TYPE				314			//通道商户类型
#define NGX_DATAPOOL_INT_MHT_TYPE				315			//内部商户类型
#define NGX_DATAPOOL_T0_GRADE_TYPE				316			//T0档位类型
#define NGX_DATAPOOL_ORI_WATER_TYPE				317			//原交易流水类型
#define NGX_DATAPOOL_AGENT_V_ACCOUNT			318			//代理商对应虚拟账号

#define NGX_DATAPOOL_VP_PLATFORM_ORG			319			//平台所属编号


#define NGX_DATAPOOL_PARTNER_SALES_PROFIT		320			//合作方营销费用比例
#define NGX_DATAPOOL_AGENT_SALES_PROFIT			321			//代理商营销费用比例

#define NGX_DATAPOOL_ORI_SYS_YEAR				322			//原交易年份


#define NGX_DATAPOOL_ACCOUNT_NAME				323			//入账人户名
#define NGX_DATAPOOL_ACCOUNT_ID					324			//入账人身份证信息
#define NGX_DATAPOOL_ACCOUNT_CARD_CODE			325			//入账人账户
#define NGX_DATAPOOL_ACCOUNT_BANK_CODE			326			//入账银行联行号
#define NGX_DATAPOOL_EXESS_ID					327			//通道商户备付金账户主键
#define NGX_DATAPOOL_CASH_STATUS				328			//提现流水状态
#define NGX_DATAPOOL_ORI_SYS_TIME				329			//原交易时间
#define NGX_DATAPOOL_CASH_RESULT				330			//提现结果
#define NGX_DATAPOOL_ORI_CASH_RESULT			331			//原交易提现结果


#define NGX_DATAPOOL_ZMK						332			//通道终端主密钥
#define NGX_DATAPOOL_EXCEPT_DESC				333			//异常说明
#define NGX_DATAPOOL_SALES_SLIP_MERCHANT_CODE	334			//签购单打印的商户号
#define NGX_DATAPOOL_SALES_SLIP_TERMINAL_CODE	335			//签购单打印的终端号
#define NGX_DATAPOOL_SALES_SLIP_MERCHANT_NAME	336			//签购单打印的商户名称



#define NGX_DATAPOOL_POSP_SIZE		(400)


#endif
