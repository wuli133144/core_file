//典型的pb结构转发机制
//吴玉杰注释 
void CDBServConn::_HandleGroupMemberInviteResponse(CImPdu* pPdu)
{   


    //基本的错误检查
    IM::Group::IMGroupMemberInviteResp msg;
    CHECK_PB_PARSE_MSG(msg.ParseFromArray(pPdu->GetBodyData(), pPdu->GetBodyLength()));
    //赋值操作
    uint32_t creator_id = msg.creator_id();
    uint32_t group_id = msg.group_id();
    uint32_t result = msg.result_code();
    string group_name = msg.group_name();
	
    char temp_str[128]; 
    snprintf(temp_str, sizeof(temp_str), "creator_id = %u, group_id = %u, list_size=%u, result=%u, user_size=%u : ", creator_id, group_id, msg.member_list().size(), result, msg.user_id().size());
    
    string user_id_str(temp_str);
    for(uint32_t i=0; i<msg.user_id().size();++i)
    {
        user_id_str += " " + int2string(msg.user_id(i));
    }
    INFO("_HandleGroupMemberInviteResponse: %s",user_id_str.c_str());

	//群成员人信息info_list
	list<uint32_t> memberList;
	for(int i=0; i<msg.member_list().size(); ++i)
	{
		uint32_t user_id = msg.member_list(i); 
		//if(user_id == creator_id) continue;
		memberList.push_back(user_id);
	}
	//memberList包含所有群成员信息
	CDbAttachData attachData((uchar_t*)msg.attach_data().c_str(), msg.attach_data().length());
    uint32_t handle = attachData.GetHandle();
    CMsgConn* pMsgConn = CImUserManager::GetInstance()->GetMsgConnByNameHandle(creator_id, handle);
    if(pMsgConn && pMsgConn->IsOpen()) 
    {
        msg.clear_attach_data();
		//msg.clear_member_list();
        pPdu->SetPBMsg(&msg);  
        pMsgConn->SendPdu(pPdu);
    }
	else 
    {
        INFO("_HandleGroupMemberInviteResponse: can't found msg_conn by creator_id = %u, handle = %u", creator_id, handle);
    }
	    //添加成功的人员信息
	    if(msg.user_id.size()==0) 
  		   return;
	    
		IM::Group::IMGroupMemberListAlterNotify msg2;
		msg2.set_notify_type(IM::BaseDefine::GROUP_INVITE_USER);
		msg2.set_group_id(group_id);
		msg2.set_creator_id(creator_id);
        msg2.set_group_name(group_name);

		for(int i=0; i<msg.user_id().size(); i++)
		{
			msg2.add_user_id(msg.user_id(i));
		}
		
		for(int i=0; i<msg.name().size(); i++)
		{
			msg2.add_user_name(msg.name(i)); 
		}        
		
		for(auto i=memberList.begin(); i!=memberList.end(); ++i)
		{
			msg2.add_member_list(*i); 
		}
   
		CImPdu pdu;
		pdu.SetPBMsg(&msg2);
		pdu.SetServiceId(SID_GROUP);
		pdu.SetCommandId(CID_GROUP_MEMBER_LIST_ALTER_NOTIFY);
		//给路由服务器发送notify
		CRouteServConn * pRouteConn = get_route_serv_conn();
		if(pRouteConn) pRouteConn->SendPdu(&pdu);
		
		//给消息服务器下的相关用户发送notify
		//发送消息的顺序
		/*
         1.首先给自己发送消息IMGroupMemberInviteResp
         2.发送CID_GROUP_MEMBER_LIST_ALTER_NOTIFY到本服务器下的用户
         3.发送CID_GROUP_MEMBER_LIST_ALTER_NOTIFY到路由给其他服务器转发
		*/

		msg2.clear_member_list();
		CImPdu pdu1;
		pdu1.SetPBMsg(&msg2);
		pdu1.SetServiceId(SID_GROUP);
		pdu1.SetCommandId(CID_GROUP_MEMBER_LIST_ALTER_NOTIFY);        
		for(auto i=memberList.begin(); i!=memberList.end(); ++i)
		{
			uint32_t user_id = *i; 
			CImUser * pUser = CImUserManager::GetInstance()->GetImUserByLoginName(int2string(user_id));
			if(pUser) 
			{
                if (user_id == creator_id)
                {
    				pUser->BroadcastPdu(&pdu1, pMsgConn);
                    DEBUG("_HandleGroupMemberInviteResponse: send notify to creator %u.",user_id);
                }
                else
                {
    				pUser->BroadcastPdu(&pdu1);
                    DEBUG("_HandleGroupMemberInviteResponse: send notify to %u.",user_id);
                }
			}
		}

        //----------------add by hfy
        if(!isTimevalid()) return;
        //-------------------------
        //-------------------------push_server--------------------//
        CPduAttachData pduAttachData(ATTACH_TYPE_NOTIFY_AND_PDU, msg2.notify_type(), pdu1.GetBodyLength(), pdu1.GetBodyData());
        IM::Server::IMGetDeviceTokenReq msg3;
		for(int i=0; i<msg2.user_id().size(); i++)
		{
			msg3.add_user_id(msg2.user_id(i));
		}
        msg3.set_attach_data(pduAttachData.GetBuffer(), pduAttachData.GetLength());
        CImPdu pdu2;
		pdu2.SetPBMsg(&msg3);
		pdu2.SetServiceId(SID_OTHER);
		pdu2.SetCommandId(CID_OTHER_GET_DEVICE_TOKEN_REQ);
        SendPdu(&pdu2); //PUSH_SERVER_MSG
          
	}   
    return;
}

