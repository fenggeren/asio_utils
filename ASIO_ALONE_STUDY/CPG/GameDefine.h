#ifndef _GAME_DEFINE_H_
#define _GAME_DEFINE_H_

#define   GATEWAYSERVER  "gateway"
#define   LOGINSERVER       "login"
#define   MATCHSERVER       "match"
#define   COMMROOMSERVER       "commroomserver"
#define   MATCHMANAGER       "matchmanager"
#define   ROUTERSERVER       "routermanager"

//游戏(房间)的类型
enum ENUM_ROOM_TYPE
{
  ENUM_ROOM_TYPE_COMM = 0,                    //常规场
  ENUM_ROOM_TYPE_SNG	= 1,                    //锦标赛下的满人即开
  ENUM_ROOM_TYPE_MTT	= 2,                    //锦标赛下的到时即开
  ENUM_ROOM_TYPE_PRAC = 3,			//邀请赛的练习场
  ENUM_ROOM_TYPE_TOUR = 4,			//邀请赛的模拟赛
	ENUM_ROOM_TYPE_QUALITY = 5,			//选拔赛的到时即开
	ENUM_ROOM_TYPE_FILLPERSON = 6,        //选拔赛的满人即开
  ENUM_ROOM_TYPE_CUSTOMCOMM = 7,        //创建的常规场比赛
  ENUM_ROOM_TYPE_CUSTOMMTT = 8,        //创建的锦标赛比赛
  // J:
  ENUM_ROOM_TYPE_MD_1 = 9,  //多轮比赛第一阶段
  ENUM_ROOM_TYPE_MD_2 = 10,  //多轮比赛第二阶段
  ENUM_ROOM_TYPE_MD_3 = 11,  //多轮比赛第三阶段, 目前支持三个阶段
};

//游戏(桌子)状态
enum ENUM_GAME_STATE
{
 	ENUM_GAME_STATE_UNKNOWN		= -1,		//未知状态
	ENUM_GAME_STATE_NONE		= 0,		//无状态
	ENUM_GAME_STATE_READY		= 1,		//准备中
	ENUM_GAME_STATE_UNDERWAY	= 2,		//进行中
	ENUM_GAME_STATE_END			= 3,		//结束中
	ENUM_GAME_STATE_WAITINGTABLE	= 4,		//等待分桌中
};


//玩家(椅子)状态
enum ENUM_USER_STATE
{
	ENUM_USER_STATE_NONE			= 0,		//无状态,椅子上无人的状态
	ENUM_USER_STATE_WAIT			= 1,		//等待中(本局进行中入座,等待下一局,或人数不够等待游戏开始)
	ENUM_USER_STATE_GAME			= 2,		//游戏中(具体在游戏中处于什么游戏状态见下一个玩家游戏状态)
	ENUM_USER_STATE_RESERVE			= 3,		//留座离位中(只在普通场,在某把椅子上,但不参与游戏)
	ENUM_USER_STATE_ELIMINATION		= 4,		//已经淘汰(只在SNG场)
	ENUM_USER_STATE_LOOKON			= 5,		//旁观(SNG和MTT场,被淘汰后的状态,不在任何一把椅子上,不参与游戏,可观看游戏,可看公共部分,不能看到任何一家的手牌)
};
//玩家游戏状态状态(当上面的玩家状态是游戏状态时)
enum ENUM_USER_GAME_STATE
{
	ENUM_USER_GAME_STATE_NONE	= 0,		//无状态,本轮没有操作过
	ENUM_USER_GAME_STATE_FOLD	= 1,		//本局已经弃牌
	ENUM_USER_GAME_STATE_CHECK	= 2,		//本轮已经check
	ENUM_USER_GAME_STATE_CALL	= 3,		//本轮已经call
	ENUM_USER_GAME_STATE_RAISE	= 4,		//本轮已经raise
	ENUM_USER_GAME_STATE_ALLIN	= 5,		//本局已经ALLIN
	ENUM_USER_GAME_STATE_MARK	= 6,		//本局已经埋牌
};

//玩家操作动作类型
enum ENUM_GAME_OPERATE_TYPE
{    
    ENUM_GAME_OPERATE_TYPE_NONE  = 0,            //无
    ENUM_GAME_OPERATE_TYPE_FOLD  = 1,            //弃牌
    ENUM_GAME_OPERATE_TYPE_CHECK = 2,            //过牌
    ENUM_GAME_OPERATE_TYPE_CALL  = 3,            //跟注
    ENUM_GAME_OPERATE_TYPE_RAISE = 4,            //加注
    ENUM_GAME_OPERATE_TYPE_ALLIN = 5,            //ALLIN
    ENUM_GAME_OPERATE_TYPE_MARK  = 6,            //埋牌
};

//游戏结果类型
enum ENUM_SCORE_TYPE
{
	ENUM_SCORE_TYPE_WIN	= 1,			//胜
	ENUM_SCORE_TYPE_LOST	= 2,			//输
	ENUM_SCORE_TYPE_DRAW	= 3,			//和
};
enum ENUM_USER_ONLINE_STATE
{
	ENUM_USER_ONLINE_STATE_ONLINE	= 1,		//
	ENUM_USER_ONLINE_STATE_CUTLINE	= 2,		//断线中,在椅子上,但断线了
};

//mtt比赛状态,是指整场比赛状态,不是某个桌子的状态
enum ENUM_MTT_MATCH_STATE
{
	ENUM_STATE_NONE			= 0,		//游戏未开始,不可报名
	ENUM_STATE_READY 		= 1,		//游戏未开始,可报名,可退赛
	ENUM_STATE_RUNING_DELAY_JOIN 	= 2,		//游戏进行中,可报名,不可退赛,可强退
	ENUM_STATE_RUNING_NOT_JOIN	= 3,		//游戏进行中,不可报名,不可退赛,可强退
	ENUM_STATE_RUNING_REWARD_ROUND	= 4,		//游戏进行中,钱圈,不可报名,不可退赛,可强退
	ENUM_STATE_RUNING_FINAL_TABLE 	= 5,		//游戏进行中,决赛桌,不可报名，不可退赛，可强退
	ENUM_STATE_FINISH		= 6,		//游戏结束
};

enum GameingState {
        GAME_STATE_PREFLOP = 0,
        GAME_STATE_FLOP,
        GAME_STATE_TURN,
        GAME_STATE_RIVER,
        GAME_STATE_POST_RIVER
};

enum matchStartConditionType
{
    TIMERCONDITION,//0. 到时即开
    FILLPERSONCONDITION,//1. 坐满即开
    MANUALCONDITION//2.由房主手动点击开始
};

enum matchRewardType
{
	UNKNOWNPOOL = -1,
    FLOATINGPOOL, //浮动奖池，根据报名人数调整奖池比例
    GUARANTEEDPOOL,//保底奖池
    GUARANTEEANDFLOATINGPOOL, //低于保底人数按照保底奖励计算，高于保底人数按照浮动奖池计算,保底只限积分
    GUARANTEECARDANDSCOREFLOATINTPOOL,//低于保底人数按照保底奖励计算，高于保底人数按照浮动奖池计算,保底资格+积分
    GUARANTEEJOINEDFULLADDONEPLAYER, // 报名人数每增加指定的N人，就增加一个奖励人数.
};//

enum CPGBounsType
{
     ENTITYAWARD = 1,//实体奖励
     TICKETAWARD = 2,//资格卡奖励
     SCOREAWARD = 4//积分奖励
};

enum CPGMatchTimerStatus
{
	TIMERWAITING = 0,//等待
	TIMERTICKING,//进行中
	TIMERPAUSEING,//暂停
	TIMERENDING,//结束
	TIMERREST//休息
};

enum CPGMatchFilterOptions
{
    FILTER_OPTIONS_ALL,
    FILTER_OPTIONS_10 ,
    FILTER_OPTIONS_50 ,
    FILTER_OPTIONS_200 ,
    FILTER_OPTIONS_600 ,
};


//    '0'    =>  array('type'=>0,'desc'=>'未知'),
//    '1'    =>  array('type'=>1,'desc'=>'比赛奖励'),
//    '2'    =>  array('type'=>2,'desc'=>'兑换资格'),
//    '3'    =>  array('type'=>3,'desc'=>'参加比赛'),
//    '4'    =>  array('type'=>4,'desc'=>'取消参赛'),
//    '5'    =>  array('type'=>5,'desc'=>'创建比赛-发起奖励'),
//    '6'    =>  array('type'=>6,'desc'=>'创建比赛-保底上缴'),
//    '7'    =>  array('type'=>7,'desc'=>'创建比赛-保底返还'),
//    '8'    =>  array('type'=>8,'desc'=>'其他'),
enum ACCOUNTLOGTYPE
{
        UNKONOWNTYPE = 0,
        MATCHREWARD = 1,
        EXCHANGESCORE = 2,
        JOINMATCH = 3,
        CANCELMATCH = 4,
        CUSTOMMATCHMATSTERREWARD = 5,
        CUSTOMMATCHGUARANTEECOMMIT = 6,
        CUSTOMMATCHGUARANTEERETURN = 7,
        OTHERREWARD = 8
};

enum registedGameType
{
    registedTicket=1,
    registedScore=2,
};

//////////////////////////////////////////////////////////////////////////////////
#define DEF_SERVER_ID_ERROR				1	//服务器id错误
#define DEF_USER_ID_ERROR				2	//用户id错误
#define DEF_SESSION_KEY_ERROR			3	//session_key错误


//////////////////////////////////////////////////////////////////////////////////
//普通场入桌返回错误码(HACLGASCommEnterTableRS)

#define DEF_COMM_ENTER_LESS_COIN_FAIL						20003	//金币扣除失败


//////////////////////////////////////////////////////////////////////////////////
//以下是二次包中的错误码
//////////////////////////////////////////////////////////////////////////////////
#define DEF_TX_NOT_HAVE_THIS_USER					1000
#define DEF_TX_TABLE_ID_ERROR						1001	//桌子号错误
#define DEF_TX_CHAIR_ID_ERROR						1002	//椅子号错误
#define DEF_TX_RESERVE_ALREADY	 					1003	//已经是留位离桌,不能再次留位离桌
#define DEF_TX_NOT_RESERVE				  			1004	//不是留位离桌状态，不能做返回桌位的操作
#define DEF_TX_NOT_HAVE_COIN 						1005	//没钱了，不让返回桌位
#define DEF_TX_RESERVE_STATUS_READY 				1006	//已经做了留位离桌标志,新一局留位离桌
#define DEF_TX_RESERVE_STATUS_NO	 				1007	//没有做留位离桌标志
#define DEF_TX_NEEDBRINGSCORE	 				1008	//携带积分不足，需要从账户扣除



#define DEF_TOUR_CANNOTSTATR_NOPERSON_ERROR             5000 //练习场锦标赛人数还不足两人不能开赛
#define DEF_TOUR_CANNOTSTATR_STARTED_ERROR              5001 //练习场锦标赛比赛已经开赛
#define DEF_TOUR_CANNOTDISS_STARTED_ERROR               5002 //练习场锦标赛比赛已经开赛,不能解散
#define DEF_TOUR_JOINEDTOUR_JOIN_ERROR                  5003 //加入练习场锦标赛比赛失败
#define DEF_TOUR_JOINEDTOUR_STARTED_ERROR               5004 //加入练习场锦标赛比赛失败




////new..........................................................................................

#define     DEF_MTT_MATCH_UNJOIN                 10000//比赛已截至报名
#define     DEF_MTT_MATCH_REPEAT_JOIN        10001//比赛已经报名，无法重复报名
#define     DEF_MTT_MATCH_NO_UNJOIN           10002//该比赛用户未报名，无法取消参赛
#define     DEF_MTT_MATCH_STARTED_UNJOIN  10003//比赛已经开始，无取消参数
#define     DEF_MTT_MATCH_ERROR_QUITMATCH    10004//该比赛用户未报名，退出比赛错误
#define     DEF_MTT_NO_MATCH                         10005//该比赛不存在
#define     DEF_SYSTEM_ERROR                          10006//系统错误，多数发生在访问HTTP的账户接口时，出现的HTTP错误
#define     DEF_NONE_PARSE_JOSON                  10007//帐号服务器返回的数据，无法解析
#define     DEF_JOIN_MATCH_ERROR                   10008//加入比赛时，发生错误，无法加入，请重试
#define     DEF_JOIN_MATCH_ERROR_SCORE       10009//加入比赛失败，积分不足
#define     DEF_JOIN_MATCH_ERROR_CARD          10010//加入比赛失败，参赛卡信息错误
#define     DEF_PARA_ERROR                                 10011//请求参数错误
#define     DEF_LOGIN_ERROR_TOKEN_INVALID    10012//登录失败，验证token失败
#define     DEF_LOGIN_FAILED                               10013//登录失败
#define     DEF_MATCH_NONE_PLAYER                  10014//用户不在该比赛中
#define     DEF_NOT_START_MATCH_ID			10015//比赛还未开始，无法获取奖励信息
#define     DEF_CREATEMATCH_EMPTYNAME   	10016//用户创建比赛时，比赛名称不能为空
#define     DEF_CREATEMATCH_NO_RULE               10017//没有该规则的赛事
#define     DEF_CUSTOM_ERROR_CONTINUETIME   10018//创建常规场比赛持续时间错误
#define     DEF_CUSTOM_ERROR_MASTERREWARD   10019//创建比赛时，房主奖励错误
#define     DEF_CUSTOMMATCH_ERROR_STATUS   10020//比赛状态错误
#define     DEF_CUSTOMMATCH_ERROR_PERSON  10021//未达到最低比赛报名人数，不满足开赛条件
#define     DEF_CUSTOMMATCH_ERROR_MASTER  10022//只有房主才能开始或解散游戏
#define     DEF_CUSTOMMATCH_ERROR_TYPE       10023//比赛类型错误
#define     DEF_CUSTOMMATCH_ERROR_SCORE    10024//创建比赛失败，积分不足
#define     DEF_CUSTOMMATCH_ERROR_CREATE   10025//创建比赛时，发生错误，无法加入，请重试
#define     DEF_CUSTOMMATCH_ERROR_STATRMIN    10026//开始比赛，报名人数不能小于钱圈奖励人数
#define     DEF_GAMESERVER_INTERNALLYERROR  10027//游戏服务器内部错误
#define     DEF_MTT_LEAVE_MATCH_ERROR                         10028//离开比赛失败
#define     DEF_CUSTOMMATCH_ERROR_MASTERMODIFYREWARD  10029//只有房主才能修改奖励
#define     DEF_CUSTOMMATCH_ERROR_REWARDPERSON  10030//自定义比赛奖励人数不正确
#define     DEF_CUSTOMMATCH_ERROR_REWARD  10031//奖励设置不合理
#define     DEF_COMMROOM_ERROR_JOIN             10032 //加入常规场出现错误，入桌失败
#define     DEF_COMMROOM_ERROR_JOINREPEAT  10033 //已经在该桌子上，无法重复加入，入桌失败
#define     DEF_COMMROOM_ERROR_NOSEAT       10034 //该房间已没有空闲座椅，请重新入桌，入桌失败
#define     DEF_CUSTOMMATCH_ERROR_NODELAYMODIFYREWARD       10035 //自定义比赛必须在截至报名后才可以修改奖励表
#define     DEF_CUSTOMMATCH_ERROR_MATCHFINISHEDMODIFYREWARD       10036 //比赛已结束，无法修改奖励分配表
#define     DEF_JOIN_MATCH_ERROR_CARDNOENOUGH       10037//加入比赛失败，资格卡不足
#define     DEF_CUSTOMMATCH_ERROR_MASTERMODIFYMAKR  10038//只有房主才能修改比赛声明
#define     DEF_CUSTOMMATCH_ERROR_SHOWMAKR  10039//未参与该比赛无法查看比赛声明
#define     DEF_COMMGAME_ERROR_BRINGSCORE_USERSTATUS  10040//留位离桌状态下不能添加筹码
#define     DEF_COMMGAME_ERROR_BRINGSCORE_TOOMUCH  10041//携带筹码量不能大于buyin的5倍
#define     DEF_MATCH_ERROR_STOPMATCHENTRY_UNJOINED  10042 //  已经截止比赛
#define     DEF_MATCH_ERROR_STOPMATCHENTRY_UNOPENED  10043 //  只有开赛后才能截止比赛
#define     DEF_MATCH_ERROR_STOPMATCHENTRY_NOTMASTER 10044 //只有小比赛的房主才可以手动截止比赛
#define     DEF_MATCH_CAN_NOT_INVITE 10046             // 不能邀请正在玩的玩家

#define DEF_SNG_FORCEQUIT                               101//SNG在比赛进行中玩家主动强制退出
#define DEF_NETWORK_ERROR_FORCEQUITGAMEUI               102//网络连接错误，退出游戏界面，返回大厅界面
#define DEF_MTT_MATCH_NO_OPEN			                104//mtt比赛未达到比赛开赛条件，返回大厅
#define DEF_MTT_USERLEAVEGAME                           105//主动离开游戏，只显示返回大厅按钮
#define DEF_MTT_USERDIEOUT                              106//用户比赛被淘汰，显示旁观和返回大厅
#define DEF_MTT_FINISHED_USERDIEOUT						107//通知第一名和第二名玩家比赛结果，显示返回大厅
#define DEF_MTT_USERLOOKUPEXITGAME					    108//提示旁观的用户，游戏结束，返回大厅
#define DEF_MTT_MTTDIVTABLEDESTORY                      109//MTT分桌时，该桌被销毁，返回大厅
#define DEF_PRAC_DISSTABLE								110//练习桌解散，返回大厅
#define DEF_ADD_MULTITABLE							    111//添加多桌，确定或取消
#define DEF_BACKTOHALL                                  112//不出提示，直接返回大厅
#define DEF_SNG_USERDIEOUT                              113//用户比赛被淘汰，显示返回大厅
#define DEF_LEAVEGAME_ANDREENTRYMATCH					114//被淘汰离开本场比赛后，重新进入赛场
#define DEF_CHANGE_MULTITABLE                           115//多桌切换
#define DEF_CUSTOMGAME_START_ACTION                     116 // 邀请赛开始
#define DEF_CUSTOMGAME_DISMISS_ACTION                   117 // 邀请赛取消

#define DEF_CREATEMTT_ERROR_NORULE                               116//没有此规则的比赛
#define DEF_CREATEMTT_ERROR_GUARANTEEREWARD           117//保底奖励必须是报名费的整数倍
#define DEF_CREATEMTT_ERROR_MATCHNAMEEMPTY            119//比赛名称不能为空
#define DEF_CREATEMTT_ERROR_MASTERREWARDPERCENT  120//比赛房主奖励百分比不正确
#define DEF_CREATEMTT_ERROR_MASTERJOINMATCH            121//房主不能加入自己创建的比赛
#define DEF_CREATEMTT_ERROR_GUARANTEEREWARDMAX    122//保底奖励不能超过最大报名人数的报名费
#define DEF_CREATEMTT_ERROR_MASTERUNJOINMATCH       123//房主不能取消报名自己创建的比赛
#define DEF_CREATEMTT_ERROR_MASTERLEAVEMATCH       124//房主不能离开自己创建的比赛



#define DEF_MESSAGEPUSH_MATCHSTART                      800//推送码，通知比赛开始
#define DEF_MESSAGEPUSH_SERVERREBOOT			      801//推送码，服务器即将重启
#define DEF_MESSAGEPUSH_MATCHANNOUNCED			802//推送码，告知用户重要比赛的开赛时间
#define DEF_MESSAGEPUSH_MATCHUNOPENED			803//推送码，告知用户报名的比赛未开赛
#define DEF_MESSAGEPUSH_CUSTOMMATCHFINISHED		804//推送码，告知用户组织的比赛已结束
#define DEF_MESSAGEPUSH_MATCHFINISHEDRESULT		805//推送码，告知用户参加比赛所获奖励


//操作的错误
#define DEF_OPERATEACTION_ERROR_COMMON                              400//操作类型错误,重新下注
#define DEF_OPERATEACTION_ERROR_ALLIN                                    401//allin数量错误或不允许allin
#define DEF_OPERATEACTION_ERROR_CHECK                                  402//不允许check
#define DEF_OPERATEACTION_ERROR_RAISE                                    403//加注数量错误或不允许加注
#define DEF_OPERATEACTION_ERROR_CALL                                      404//call数量错误
#define DEF_OPERATEACTION_ERROR_CURRENTUSER                      405	//还没有轮到您操作
#define DEF_OPERATEACTION_ERROR_COINAMOUNT_GREATER       406	//下注筹码数量大于拥有筹码量
#define DEF_OPERATEACTION_ERROR_COINAMOUNT_SMALLER	    407	//下注筹码数量小于零
#define DEF_OPERATEACTION_ERROR_CHAIRNO                                408//椅子号不正确
#define DEF_OPERATEACTION_ERROR_GAMESTATUS                                409//游戏不在进行过程中，无法操作
#define DEF_OPERATEACTION_ERROR_FOLDED                                  410	//用户已经是Fold状态


#endif//_GAME_DEFINE_H_
