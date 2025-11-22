




/* ---------------- method closures -------------- */
#ifndef CK_TEMPLATES_ONLY
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY

    struct Closure_Main::initDone_2_closure : public SDAG::Closure {
      

      initDone_2_closure() {
        init();
      }
      initDone_2_closure(CkMigrateMessage*) {
        init();
      }
            void pup(PUP::er& __p) {
        packClosure(__p);
      }
      virtual ~initDone_2_closure() {
      }
      PUPable_decl(SINGLE_ARG(initDone_2_closure));
    };
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY

    struct Closure_Main::nodeDone_3_closure : public SDAG::Closure {
      

      nodeDone_3_closure() {
        init();
      }
      nodeDone_3_closure(CkMigrateMessage*) {
        init();
      }
            void pup(PUP::er& __p) {
        packClosure(__p);
      }
      virtual ~nodeDone_3_closure() {
      }
      PUPable_decl(SINGLE_ARG(nodeDone_3_closure));
    };
#endif /* CK_TEMPLATES_ONLY */


/* ---------------- method closures -------------- */
#ifndef CK_TEMPLATES_ONLY
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY

    struct Closure_NodeManager::registerWorker_2_closure : public SDAG::Closure {
      

      registerWorker_2_closure() {
        init();
      }
      registerWorker_2_closure(CkMigrateMessage*) {
        init();
      }
            void pup(PUP::er& __p) {
        packClosure(__p);
      }
      virtual ~registerWorker_2_closure() {
      }
      PUPable_decl(SINGLE_ARG(registerWorker_2_closure));
    };
#endif /* CK_TEMPLATES_ONLY */


/* ---------------- method closures -------------- */
#ifndef CK_TEMPLATES_ONLY
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY

    struct Closure_Worker::startStep_2_closure : public SDAG::Closure {
      

      startStep_2_closure() {
        init();
      }
      startStep_2_closure(CkMigrateMessage*) {
        init();
      }
            void pup(PUP::er& __p) {
        packClosure(__p);
      }
      virtual ~startStep_2_closure() {
      }
      PUPable_decl(SINGLE_ARG(startStep_2_closure));
    };
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
#endif /* CK_TEMPLATES_ONLY */


/* DEFS: readonly CProxy_Main mainProxy;
 */
extern CProxy_Main mainProxy;
#ifndef CK_TEMPLATES_ONLY
extern "C" void __xlater_roPup_mainProxy(void *_impl_pup_er) {
  PUP::er &_impl_p=*(PUP::er *)_impl_pup_er;
  _impl_p|mainProxy;
}
#endif /* CK_TEMPLATES_ONLY */

/* DEFS: readonly CProxy_NodeManager nodeManagerProxy;
 */
extern CProxy_NodeManager nodeManagerProxy;
#ifndef CK_TEMPLATES_ONLY
extern "C" void __xlater_roPup_nodeManagerProxy(void *_impl_pup_er) {
  PUP::er &_impl_p=*(PUP::er *)_impl_pup_er;
  _impl_p|nodeManagerProxy;
}
#endif /* CK_TEMPLATES_ONLY */

/* DEFS: readonly CProxy_Worker workerProxy;
 */
extern CProxy_Worker workerProxy;
#ifndef CK_TEMPLATES_ONLY
extern "C" void __xlater_roPup_workerProxy(void *_impl_pup_er) {
  PUP::er &_impl_p=*(PUP::er *)_impl_pup_er;
  _impl_p|workerProxy;
}
#endif /* CK_TEMPLATES_ONLY */

/* DEFS: readonly int vectorSize;
 */
extern int vectorSize;
#ifndef CK_TEMPLATES_ONLY
extern "C" void __xlater_roPup_vectorSize(void *_impl_pup_er) {
  PUP::er &_impl_p=*(PUP::er *)_impl_pup_er;
  _impl_p|vectorSize;
}
#endif /* CK_TEMPLATES_ONLY */

/* DEFS: readonly int numCharesPerNode;
 */
extern int numCharesPerNode;
#ifndef CK_TEMPLATES_ONLY
extern "C" void __xlater_roPup_numCharesPerNode(void *_impl_pup_er) {
  PUP::er &_impl_p=*(PUP::er *)_impl_pup_er;
  _impl_p|numCharesPerNode;
}
#endif /* CK_TEMPLATES_ONLY */

/* DEFS: mainchare Main: Chare{
Main(CkArgMsg* impl_msg);
void initDone();
void nodeDone();
};
 */
#ifndef CK_TEMPLATES_ONLY
 int CkIndex_Main::__idx=0;
#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
/* DEFS: Main(CkArgMsg* impl_msg);
 */
CkChareID CProxy_Main::ckNew(CkArgMsg* impl_msg, int impl_onPE)
{
  CkChareID impl_ret;
  CkCreateChare(CkIndex_Main::__idx, CkIndex_Main::idx_Main_CkArgMsg(), impl_msg, &impl_ret, impl_onPE);
  return impl_ret;
}
void CProxy_Main::ckNew(CkArgMsg* impl_msg, CkChareID* pcid, int impl_onPE)
{
  CkCreateChare(CkIndex_Main::__idx, CkIndex_Main::idx_Main_CkArgMsg(), impl_msg, pcid, impl_onPE);
}

// Entry point registration function
int CkIndex_Main::reg_Main_CkArgMsg() {
  int epidx = CkRegisterEp("Main(CkArgMsg* impl_msg)",
      reinterpret_cast<CkCallFnPtr>(_call_Main_CkArgMsg), CMessage_CkArgMsg::__idx, __idx, 0);
  CkRegisterMessagePupFn(epidx, (CkMessagePupFn)CkArgMsg::ckDebugPup);
  return epidx;
}

void CkIndex_Main::_call_Main_CkArgMsg(void* impl_msg, void* impl_obj_void)
{
  Main* impl_obj = static_cast<Main*>(impl_obj_void);
  new (impl_obj_void) Main((CkArgMsg*)impl_msg);
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: void initDone();
 */
void CProxy_Main::initDone(const CkEntryOptions *impl_e_opts)
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg(impl_e_opts);
  if (ckIsDelegated()) {
    int destPE=CkChareMsgPrep(CkIndex_Main::idx_initDone_void(), impl_msg, &ckGetChareID());
    if (destPE!=-1) ckDelegatedTo()->ChareSend(ckDelegatedPtr(),CkIndex_Main::idx_initDone_void(), impl_msg, &ckGetChareID(),destPE);
  } else {
    CkSendMsg(CkIndex_Main::idx_initDone_void(), impl_msg, &ckGetChareID(),0);
  }
}

// Entry point registration function
int CkIndex_Main::reg_initDone_void() {
  int epidx = CkRegisterEp("initDone()",
      reinterpret_cast<CkCallFnPtr>(_call_initDone_void), 0, __idx, 0);
  return epidx;
}

void CkIndex_Main::_call_initDone_void(void* impl_msg, void* impl_obj_void)
{
  Main* impl_obj = static_cast<Main*>(impl_obj_void);
  impl_obj->initDone();
  if(UsrToEnv(impl_msg)->isVarSysMsg() == 0)
    CkFreeSysMsg(impl_msg);
}
PUPable_def(SINGLE_ARG(Closure_Main::initDone_2_closure))
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: void nodeDone();
 */
void CProxy_Main::nodeDone(const CkEntryOptions *impl_e_opts)
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg(impl_e_opts);
  if (ckIsDelegated()) {
    int destPE=CkChareMsgPrep(CkIndex_Main::idx_nodeDone_void(), impl_msg, &ckGetChareID());
    if (destPE!=-1) ckDelegatedTo()->ChareSend(ckDelegatedPtr(),CkIndex_Main::idx_nodeDone_void(), impl_msg, &ckGetChareID(),destPE);
  } else {
    CkSendMsg(CkIndex_Main::idx_nodeDone_void(), impl_msg, &ckGetChareID(),0);
  }
}

// Entry point registration function
int CkIndex_Main::reg_nodeDone_void() {
  int epidx = CkRegisterEp("nodeDone()",
      reinterpret_cast<CkCallFnPtr>(_call_nodeDone_void), 0, __idx, 0);
  return epidx;
}

void CkIndex_Main::_call_nodeDone_void(void* impl_msg, void* impl_obj_void)
{
  Main* impl_obj = static_cast<Main*>(impl_obj_void);
  impl_obj->nodeDone();
  if(UsrToEnv(impl_msg)->isVarSysMsg() == 0)
    CkFreeSysMsg(impl_msg);
}
PUPable_def(SINGLE_ARG(Closure_Main::nodeDone_3_closure))
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
void CkIndex_Main::__register(const char *s, size_t size) {
  __idx = CkRegisterChare(s, size, TypeMainChare);
  CkRegisterBase(__idx, CkIndex_Chare::__idx);
  // REG: Main(CkArgMsg* impl_msg);
  idx_Main_CkArgMsg();
  CkRegisterMainChare(__idx, idx_Main_CkArgMsg());

  // REG: void initDone();
  idx_initDone_void();

  // REG: void nodeDone();
  idx_nodeDone_void();

}
#endif /* CK_TEMPLATES_ONLY */

/* DEFS: group NodeManager: IrrGroup{
NodeManager(int nBytes, const char *id);
void registerWorker();
};
 */
#ifndef CK_TEMPLATES_ONLY
 int CkIndex_NodeManager::__idx=0;
#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
/* DEFS: NodeManager(int nBytes, const char *id);
 */
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: void registerWorker();
 */
void CProxyElement_NodeManager::registerWorker(const CkEntryOptions *impl_e_opts)
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg(impl_e_opts);
  if (ckIsDelegated()) {
     CkGroupMsgPrep(CkIndex_NodeManager::idx_registerWorker_void(), impl_msg, ckGetGroupID());
     ckDelegatedTo()->GroupSend(ckDelegatedPtr(),CkIndex_NodeManager::idx_registerWorker_void(), impl_msg, ckGetGroupPe(), ckGetGroupID());
  } else {
    CkSendMsgBranch(CkIndex_NodeManager::idx_registerWorker_void(), impl_msg, ckGetGroupPe(), ckGetGroupID(),0);
  }
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: NodeManager(int nBytes, const char *id);
 */
CkGroupID CProxy_NodeManager::ckNew(int nBytes, const char *id, const CkEntryOptions *impl_e_opts)
{
  //Marshall: int nBytes, const char *id
  int impl_off=0;
  int impl_arrstart=0;
  int impl_off_id, impl_cnt_id;
  impl_off_id=impl_off=CK_ALIGN(impl_off,sizeof(char));
  impl_off+=(impl_cnt_id=sizeof(char)*(nBytes));
  { //Find the size of the PUP'd data
    PUP::sizer implP;
    implP|nBytes;
    implP|impl_off_id;
    implP|impl_cnt_id;
    impl_arrstart=CK_ALIGN(implP.size(),16);
    impl_off+=impl_arrstart;
  }
  CkMarshallMsg *impl_msg=CkAllocateMarshallMsg(impl_off,impl_e_opts);
  { //Copy over the PUP'd data
    PUP::toMem implP((void *)impl_msg->msgBuf);
    implP|nBytes;
    implP|impl_off_id;
    implP|impl_cnt_id;
  }
  char *impl_buf=impl_msg->msgBuf+impl_arrstart;
  memcpy(impl_buf+impl_off_id,id,impl_cnt_id);
  UsrToEnv(impl_msg)->setMsgtype(BocInitMsg);
  CkGroupID gId = CkCreateGroup(CkIndex_NodeManager::__idx, CkIndex_NodeManager::idx_NodeManager_marshall1(), impl_msg);
  return gId;
}
  CProxy_NodeManager::CProxy_NodeManager(int nBytes, const char *id, const CkEntryOptions *impl_e_opts)
{
  //Marshall: int nBytes, const char *id
  int impl_off=0;
  int impl_arrstart=0;
  int impl_off_id, impl_cnt_id;
  impl_off_id=impl_off=CK_ALIGN(impl_off,sizeof(char));
  impl_off+=(impl_cnt_id=sizeof(char)*(nBytes));
  { //Find the size of the PUP'd data
    PUP::sizer implP;
    implP|nBytes;
    implP|impl_off_id;
    implP|impl_cnt_id;
    impl_arrstart=CK_ALIGN(implP.size(),16);
    impl_off+=impl_arrstart;
  }
  CkMarshallMsg *impl_msg=CkAllocateMarshallMsg(impl_off,impl_e_opts);
  { //Copy over the PUP'd data
    PUP::toMem implP((void *)impl_msg->msgBuf);
    implP|nBytes;
    implP|impl_off_id;
    implP|impl_cnt_id;
  }
  char *impl_buf=impl_msg->msgBuf+impl_arrstart;
  memcpy(impl_buf+impl_off_id,id,impl_cnt_id);
  UsrToEnv(impl_msg)->setMsgtype(BocInitMsg);
  ckSetGroupID(CkCreateGroup(CkIndex_NodeManager::__idx, CkIndex_NodeManager::idx_NodeManager_marshall1(), impl_msg));
}

// Entry point registration function
int CkIndex_NodeManager::reg_NodeManager_marshall1() {
  int epidx = CkRegisterEp("NodeManager(int nBytes, const char *id)",
      reinterpret_cast<CkCallFnPtr>(_call_NodeManager_marshall1), CkMarshallMsg::__idx, __idx, 0+CK_EP_NOKEEP);
  CkRegisterMarshallUnpackFn(epidx, _callmarshall_NodeManager_marshall1);
  CkRegisterMessagePupFn(epidx, _marshallmessagepup_NodeManager_marshall1);

  return epidx;
}

void CkIndex_NodeManager::_call_NodeManager_marshall1(void* impl_msg, void* impl_obj_void)
{
  NodeManager* impl_obj = static_cast<NodeManager*>(impl_obj_void);
  CkMarshallMsg *impl_msg_typed=(CkMarshallMsg *)impl_msg;
  char *impl_buf=impl_msg_typed->msgBuf;
  envelope *env = UsrToEnv(impl_msg_typed);
  /*Unmarshall pup'd fields: int nBytes, const char *id*/
  PUP::fromMem implP(impl_buf);
  PUP::detail::TemporaryObjectHolder<int> nBytes;
  implP|nBytes;
  int impl_off_id, impl_cnt_id;
  implP|impl_off_id;
  implP|impl_cnt_id;
  impl_buf+=CK_ALIGN(implP.size(),16);
  /*Unmarshall arrays:*/
  char *id=(char *)(impl_buf+impl_off_id);
  new (impl_obj_void) NodeManager(std::move(nBytes.t), id);
}
int CkIndex_NodeManager::_callmarshall_NodeManager_marshall1(char* impl_buf, void* impl_obj_void) {
  NodeManager* impl_obj = static_cast<NodeManager*>(impl_obj_void);
  envelope *env = UsrToEnv(impl_buf);
  /*Unmarshall pup'd fields: int nBytes, const char *id*/
  PUP::fromMem implP(impl_buf);
  PUP::detail::TemporaryObjectHolder<int> nBytes;
  implP|nBytes;
  int impl_off_id, impl_cnt_id;
  implP|impl_off_id;
  implP|impl_cnt_id;
  impl_buf+=CK_ALIGN(implP.size(),16);
  /*Unmarshall arrays:*/
  char *id=(char *)(impl_buf+impl_off_id);
  new (impl_obj_void) NodeManager(std::move(nBytes.t), id);
  return implP.size();
}
void CkIndex_NodeManager::_marshallmessagepup_NodeManager_marshall1(PUP::er &implDestP,void *impl_msg) {
  CkMarshallMsg *impl_msg_typed=(CkMarshallMsg *)impl_msg;
  char *impl_buf=impl_msg_typed->msgBuf;
  envelope *env = UsrToEnv(impl_msg_typed);
  /*Unmarshall pup'd fields: int nBytes, const char *id*/
  PUP::fromMem implP(impl_buf);
  PUP::detail::TemporaryObjectHolder<int> nBytes;
  implP|nBytes;
  int impl_off_id, impl_cnt_id;
  implP|impl_off_id;
  implP|impl_cnt_id;
  impl_buf+=CK_ALIGN(implP.size(),16);
  /*Unmarshall arrays:*/
  char *id=(char *)(impl_buf+impl_off_id);
  if (implDestP.hasComments()) implDestP.comment("nBytes");
  implDestP|nBytes;
  if (implDestP.hasComments()) implDestP.comment("id");
  implDestP.synchronize(PUP::sync_begin_array);
  for (int impl_i=0;impl_i*(sizeof(*id))<impl_cnt_id;impl_i++) {
    implDestP.synchronize(PUP::sync_item);
    implDestP|id[impl_i];
  }
  implDestP.synchronize(PUP::sync_end_array);
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: void registerWorker();
 */
void CProxy_NodeManager::registerWorker(const CkEntryOptions *impl_e_opts)
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg(impl_e_opts);
  if (ckIsDelegated()) {
     CkGroupMsgPrep(CkIndex_NodeManager::idx_registerWorker_void(), impl_msg, ckGetGroupID());
     ckDelegatedTo()->GroupBroadcast(ckDelegatedPtr(),CkIndex_NodeManager::idx_registerWorker_void(), impl_msg, ckGetGroupID());
  } else CkBroadcastMsgBranch(CkIndex_NodeManager::idx_registerWorker_void(), impl_msg, ckGetGroupID(),0);
}
void CProxy_NodeManager::registerWorker(int npes, int *pes, const CkEntryOptions *impl_e_opts) {
  void *impl_msg = CkAllocSysMsg(impl_e_opts);
  CkSendMsgBranchMulti(CkIndex_NodeManager::idx_registerWorker_void(), impl_msg, ckGetGroupID(), npes, pes,0);
}
void CProxy_NodeManager::registerWorker(CmiGroup &grp, const CkEntryOptions *impl_e_opts) {
  void *impl_msg = CkAllocSysMsg(impl_e_opts);
  CkSendMsgBranchGroup(CkIndex_NodeManager::idx_registerWorker_void(), impl_msg, ckGetGroupID(), grp,0);
}

// Entry point registration function
int CkIndex_NodeManager::reg_registerWorker_void() {
  int epidx = CkRegisterEp("registerWorker()",
      reinterpret_cast<CkCallFnPtr>(_call_registerWorker_void), 0, __idx, 0);
  return epidx;
}

void CkIndex_NodeManager::_call_registerWorker_void(void* impl_msg, void* impl_obj_void)
{
  NodeManager* impl_obj = static_cast<NodeManager*>(impl_obj_void);
  impl_obj->registerWorker();
  if(UsrToEnv(impl_msg)->isVarSysMsg() == 0)
    CkFreeSysMsg(impl_msg);
}
PUPable_def(SINGLE_ARG(Closure_NodeManager::registerWorker_2_closure))
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: NodeManager(int nBytes, const char *id);
 */
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: void registerWorker();
 */
void CProxySection_NodeManager::registerWorker(const CkEntryOptions *impl_e_opts)
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg(impl_e_opts);
  if (ckIsDelegated()) {
     ckDelegatedTo()->GroupSectionSend(ckDelegatedPtr(),CkIndex_NodeManager::idx_registerWorker_void(), impl_msg, ckGetNumSections(), ckGetSectionIDs());
  } else {
    void *impl_msg_tmp;
    for (int i=0; i<ckGetNumSections(); ++i) {
       impl_msg_tmp= (i<ckGetNumSections()-1) ? CkCopyMsg((void **) &impl_msg):impl_msg;
       CkSendMsgBranchMulti(CkIndex_NodeManager::idx_registerWorker_void(), impl_msg_tmp, ckGetGroupIDn(i), ckGetNumElements(i), ckGetElements(i),0);
    }
  }
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
void CkIndex_NodeManager::__register(const char *s, size_t size) {
  __idx = CkRegisterChare(s, size, TypeGroup);
  CkRegisterBase(__idx, CkIndex_IrrGroup::__idx);
   CkRegisterGroupIrr(__idx,NodeManager::isIrreducible());
  // REG: NodeManager(int nBytes, const char *id);
  idx_NodeManager_marshall1();

  // REG: void registerWorker();
  idx_registerWorker_void();

}
#endif /* CK_TEMPLATES_ONLY */

/* DEFS: array Worker: ArrayElement{
Worker();
void startStep();
Worker(CkMigrateMessage* impl_msg);
};
 */
#ifndef CK_TEMPLATES_ONLY
 int CkIndex_Worker::__idx=0;
#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
void CProxySection_Worker::contribute(CkSectionInfo &sid, int userData, int fragSize)
{
   CkArray *ckarr = CProxy_CkArray(sid.get_aid()).ckLocalBranch();
   CkMulticastMgr *mCastGrp = CProxy_CkMulticastMgr(ckarr->getmCastMgr()).ckLocalBranch();
   mCastGrp->contribute(sid, userData, fragSize);
}

void CProxySection_Worker::contribute(int dataSize,void *data,CkReduction::reducerType type, CkSectionInfo &sid, int userData, int fragSize)
{
   CkArray *ckarr = CProxy_CkArray(sid.get_aid()).ckLocalBranch();
   CkMulticastMgr *mCastGrp = CProxy_CkMulticastMgr(ckarr->getmCastMgr()).ckLocalBranch();
   mCastGrp->contribute(dataSize, data, type, sid, userData, fragSize);
}

template <typename T>
void CProxySection_Worker::contribute(std::vector<T> &data, CkReduction::reducerType type, CkSectionInfo &sid, int userData, int fragSize)
{
   CkArray *ckarr = CProxy_CkArray(sid.get_aid()).ckLocalBranch();
   CkMulticastMgr *mCastGrp = CProxy_CkMulticastMgr(ckarr->getmCastMgr()).ckLocalBranch();
   mCastGrp->contribute(data, type, sid, userData, fragSize);
}

void CProxySection_Worker::contribute(CkSectionInfo &sid, const CkCallback &cb, int userData, int fragSize)
{
   CkArray *ckarr = CProxy_CkArray(sid.get_aid()).ckLocalBranch();
   CkMulticastMgr *mCastGrp = CProxy_CkMulticastMgr(ckarr->getmCastMgr()).ckLocalBranch();
   mCastGrp->contribute(sid, cb, userData, fragSize);
}

void CProxySection_Worker::contribute(int dataSize,void *data,CkReduction::reducerType type, CkSectionInfo &sid, const CkCallback &cb, int userData, int fragSize)
{
   CkArray *ckarr = CProxy_CkArray(sid.get_aid()).ckLocalBranch();
   CkMulticastMgr *mCastGrp = CProxy_CkMulticastMgr(ckarr->getmCastMgr()).ckLocalBranch();
   mCastGrp->contribute(dataSize, data, type, sid, cb, userData, fragSize);
}

template <typename T>
void CProxySection_Worker::contribute(std::vector<T> &data, CkReduction::reducerType type, CkSectionInfo &sid, const CkCallback &cb, int userData, int fragSize)
{
   CkArray *ckarr = CProxy_CkArray(sid.get_aid()).ckLocalBranch();
   CkMulticastMgr *mCastGrp = CProxy_CkMulticastMgr(ckarr->getmCastMgr()).ckLocalBranch();
   mCastGrp->contribute(data, type, sid, cb, userData, fragSize);
}

#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
/* DEFS: Worker();
 */
void CProxyElement_Worker::insert(int onPE, const CkEntryOptions *impl_e_opts)
{ 
   void *impl_msg = CkAllocSysMsg(impl_e_opts);
   UsrToEnv(impl_msg)->setMsgtype(ArrayEltInitMsg);
   ckInsert((CkArrayMessage *)impl_msg,CkIndex_Worker::idx_Worker_void(),onPE);
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: void startStep();
 */
void CProxyElement_Worker::startStep(const CkEntryOptions *impl_e_opts) 
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg(impl_e_opts);
  UsrToEnv(impl_msg)->setMsgtype(ForArrayEltMsg);
  CkArrayMessage *impl_amsg=(CkArrayMessage *)impl_msg;
  impl_amsg->array_setIfNotThere(CkArray_IfNotThere_buffer);
  ckSend(impl_amsg, CkIndex_Worker::idx_startStep_void(),0);
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: Worker(CkMigrateMessage* impl_msg);
 */
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: Worker();
 */
CkArrayID CProxy_Worker::ckNew(const CkArrayOptions &opts, const CkEntryOptions *impl_e_opts)
{
  void *impl_msg = CkAllocSysMsg(impl_e_opts);
  UsrToEnv(impl_msg)->setMsgtype(ArrayEltInitMsg);
  CkArrayID gId = ckCreateArray((CkArrayMessage *)impl_msg, CkIndex_Worker::idx_Worker_void(), opts);
  return gId;
}
void CProxy_Worker::ckNew(const CkArrayOptions &opts, CkCallback _ck_array_creation_cb, const CkEntryOptions *impl_e_opts)
{
  void *impl_msg = CkAllocSysMsg(impl_e_opts);
  UsrToEnv(impl_msg)->setMsgtype(ArrayEltInitMsg);
  CkSendAsyncCreateArray(CkIndex_Worker::idx_Worker_void(), _ck_array_creation_cb, opts, impl_msg);
}
CkArrayID CProxy_Worker::ckNew(const int s1, const CkEntryOptions *impl_e_opts)
{
  void *impl_msg = CkAllocSysMsg(impl_e_opts);
  CkArrayOptions opts(s1);
  UsrToEnv(impl_msg)->setMsgtype(ArrayEltInitMsg);
  CkArrayID gId = ckCreateArray((CkArrayMessage *)impl_msg, CkIndex_Worker::idx_Worker_void(), opts);
  return gId;
}
void CProxy_Worker::ckNew(const int s1, CkCallback _ck_array_creation_cb, const CkEntryOptions *impl_e_opts)
{
  void *impl_msg = CkAllocSysMsg(impl_e_opts);
  CkArrayOptions opts(s1);
  UsrToEnv(impl_msg)->setMsgtype(ArrayEltInitMsg);
  CkSendAsyncCreateArray(CkIndex_Worker::idx_Worker_void(), _ck_array_creation_cb, opts, impl_msg);
}

// Entry point registration function
int CkIndex_Worker::reg_Worker_void() {
  int epidx = CkRegisterEp("Worker()",
      reinterpret_cast<CkCallFnPtr>(_call_Worker_void), 0, __idx, 0);
  return epidx;
}

void CkIndex_Worker::_call_Worker_void(void* impl_msg, void* impl_obj_void)
{
  Worker* impl_obj = static_cast<Worker*>(impl_obj_void);
  new (impl_obj_void) Worker();
  if(UsrToEnv(impl_msg)->isVarSysMsg() == 0)
    CkFreeSysMsg(impl_msg);
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: void startStep();
 */
void CProxy_Worker::startStep(const CkEntryOptions *impl_e_opts) 
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg(impl_e_opts);
  UsrToEnv(impl_msg)->setMsgtype(ForArrayEltMsg);
  CkArrayMessage *impl_amsg=(CkArrayMessage *)impl_msg;
  impl_amsg->array_setIfNotThere(CkArray_IfNotThere_buffer);
  ckBroadcast(impl_amsg, CkIndex_Worker::idx_startStep_void(),0);
}

// Entry point registration function
int CkIndex_Worker::reg_startStep_void() {
  int epidx = CkRegisterEp("startStep()",
      reinterpret_cast<CkCallFnPtr>(_call_startStep_void), 0, __idx, 0);
  return epidx;
}

void CkIndex_Worker::_call_startStep_void(void* impl_msg, void* impl_obj_void)
{
  Worker* impl_obj = static_cast<Worker*>(impl_obj_void);
  impl_obj->startStep();
  if(UsrToEnv(impl_msg)->isVarSysMsg() == 0)
    CkFreeSysMsg(impl_msg);
}
PUPable_def(SINGLE_ARG(Closure_Worker::startStep_2_closure))
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: Worker(CkMigrateMessage* impl_msg);
 */

// Entry point registration function
int CkIndex_Worker::reg_Worker_CkMigrateMessage() {
  int epidx = CkRegisterEp("Worker(CkMigrateMessage* impl_msg)",
      reinterpret_cast<CkCallFnPtr>(_call_Worker_CkMigrateMessage), 0, __idx, 0);
  return epidx;
}

void CkIndex_Worker::_call_Worker_CkMigrateMessage(void* impl_msg, void* impl_obj_void)
{
  call_migration_constructor<Worker> c = impl_obj_void;
  c((CkMigrateMessage*)impl_msg);
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: Worker();
 */
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: void startStep();
 */
void CProxySection_Worker::startStep(const CkEntryOptions *impl_e_opts) 
{
  ckCheck();
  void *impl_msg = CkAllocSysMsg(impl_e_opts);
  UsrToEnv(impl_msg)->setMsgtype(ForArrayEltMsg);
  CkArrayMessage *impl_amsg=(CkArrayMessage *)impl_msg;
  impl_amsg->array_setIfNotThere(CkArray_IfNotThere_buffer);
  ckSend(impl_amsg, CkIndex_Worker::idx_startStep_void(),0);
}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
/* DEFS: Worker(CkMigrateMessage* impl_msg);
 */
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
void CkIndex_Worker::__register(const char *s, size_t size) {
  __idx = CkRegisterChare(s, size, TypeArray);
  CkRegisterArrayDimensions(__idx, 1);
  CkRegisterBase(__idx, CkIndex_ArrayElement::__idx);
  // REG: Worker();
  idx_Worker_void();
  CkRegisterDefaultCtor(__idx, idx_Worker_void());

  // REG: void startStep();
  idx_startStep_void();

  // REG: Worker(CkMigrateMessage* impl_msg);
  idx_Worker_CkMigrateMessage();
  CkRegisterMigCtor(__idx, idx_Worker_CkMigrateMessage());

}
#endif /* CK_TEMPLATES_ONLY */

#ifndef CK_TEMPLATES_ONLY
void _registernccl_integration(void)
{
  static int _done = 0; if(_done) return; _done = 1;
  CkRegisterReadonly("mainProxy","CProxy_Main",sizeof(mainProxy),(void *) &mainProxy,__xlater_roPup_mainProxy);

  CkRegisterReadonly("nodeManagerProxy","CProxy_NodeManager",sizeof(nodeManagerProxy),(void *) &nodeManagerProxy,__xlater_roPup_nodeManagerProxy);

  CkRegisterReadonly("workerProxy","CProxy_Worker",sizeof(workerProxy),(void *) &workerProxy,__xlater_roPup_workerProxy);

  CkRegisterReadonly("vectorSize","int",sizeof(vectorSize),(void *) &vectorSize,__xlater_roPup_vectorSize);

  CkRegisterReadonly("numCharesPerNode","int",sizeof(numCharesPerNode),(void *) &numCharesPerNode,__xlater_roPup_numCharesPerNode);

/* REG: mainchare Main: Chare{
Main(CkArgMsg* impl_msg);
void initDone();
void nodeDone();
};
*/
  CkIndex_Main::__register("Main", sizeof(Main));

/* REG: group NodeManager: IrrGroup{
NodeManager(int nBytes, const char *id);
void registerWorker();
};
*/
  CkIndex_NodeManager::__register("NodeManager", sizeof(NodeManager));

/* REG: array Worker: ArrayElement{
Worker();
void startStep();
Worker(CkMigrateMessage* impl_msg);
};
*/
  CkIndex_Worker::__register("Worker", sizeof(Worker));

}
extern "C" void CkRegisterMainModule(void) {
  _registernccl_integration();
}
#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
template <>
void CBase_Main::virtual_pup(PUP::er &p) {
    recursive_pup<Main>(dynamic_cast<Main*>(this), p);
}
#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
template <>
void CBase_NodeManager::virtual_pup(PUP::er &p) {
    recursive_pup<NodeManager>(dynamic_cast<NodeManager*>(this), p);
}
#endif /* CK_TEMPLATES_ONLY */
#ifndef CK_TEMPLATES_ONLY
template <>
void CBase_Worker::virtual_pup(PUP::er &p) {
    recursive_pup<Worker>(dynamic_cast<Worker*>(this), p);
}
#endif /* CK_TEMPLATES_ONLY */
