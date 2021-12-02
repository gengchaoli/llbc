// The MIT License (MIT)

// Copyright (c) 2013 lailongwei<lailongwei@126.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), to deal in 
// the Software without restriction, including without limitation the rights to 
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of 
// the Software, and to permit persons to whom the Software is furnished to do so, 
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all 
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS 
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef __LLBC_CORE_EVENT_EVENT_MANAGER_H__
#define __LLBC_CORE_EVENT_EVENT_MANAGER_H__

#include "llbc/common/Common.h"
#include "llbc/core/utils/Common.h"

__LLBC_NS_BEGIN
class LLBC_Event;
__LLBC_NS_END

__LLBC_NS_BEGIN

/**
 * \brief The event manager class encapsulation.
 */
class LLBC_EXPORT LLBC_EventManager
{
public:
    /**
     * Ctor & Dtor.
     */
    LLBC_EventManager();
    virtual ~LLBC_EventManager();

public:
    /**
     * Add event deleg.
     * @param[in] id         - event Id.
     * @param[in] listener   - event deleg.
     * @param[in] boundStub - the binded stub, if not specified, will auto gen stub.
     * @return LLBC_ListenerStub - return if failed, otherwise return validate stub.
     */
    virtual LLBC_ListenerStub AddListener(int id,
                                          void (*listener)(LLBC_Event &),
                                          const LLBC_ListenerStub &boundStub = LLBC_INVALID_LISTENER_STUB);

    /**
     * Add event deleg.
     * @param[in] id         - event Id.
     * @param[in] obj        - object.
     * @param[in] listener   - deleg.
     * @param[in] boundStub - the binded stub, if not specified, will auto gen stub.
     * @return LLBC_ListenerStub - return LLBC_INVALID_LISTENER_STUB if failed, otherwise return validate stub.
     */
    template <typename ObjectType>
    LLBC_ListenerStub AddListener(int id,
                                  ObjectType *obj,
                                  void (ObjectType::*listener)(LLBC_Event &),
                                  const LLBC_ListenerStub &boundStub = LLBC_INVALID_LISTENER_STUB);
    /**
     * Add event deleg.
     * @param[in] id        - event Id.
     * @param[in] listener  - event deleg.
     * @param[in] boundStub - the bound stub, if not specified, will auto gen stub.
     * @return LLBC_ListenerStub - return LLBC_INVALID_LISTENER_STUB if failed, otherwise return validate stub.
     */
    virtual LLBC_ListenerStub AddListener(int id,
                                          const LLBC_NewDelegate<void(LLBC_Event &)> &listener,
                                          const LLBC_ListenerStub &boundStub = LLBC_INVALID_LISTENER_STUB);

    /**
     * Add event listener.
     * @param[in] id - event Id.
     * @param[in] listener - event listener.
     * @return LLBC_ListenerStub - return LLBC_INAVLID_LISTENER_STUB if failed, otherwise return validate stub.
     */
    virtual LLBC_ListenerStub AddListener(int id,
                                          LLBC_EventListener *listener,
                                          const LLBC_ListenerStub &boundStub = LLBC_INVALID_LISTENER_STUB);

    /**
     * Remove event deleg.
     * @param[in] id - event Id.
     * @return int - success if return LLBC_OK, otherwise return LLBC_FAILED.
     *               specially, if return LLBC_FAILED,  and fetch the last error is pending,
     *               it means operation will success on later, but pending at now.
     */
    virtual int RemoveListener(int id);

    /**
     * Remove event deleg using deleg stub.
     * @param[in] stub - event deleg stub.
     * @return int - success if return LLBC_OK, otherwise return LLBC_FAILED,
     *               specially, if return LLBC_FAILED, and fetch the last error is pending,
     *               it means operation will success on later, but pending at now.
     */
    virtual int RemoveListener(const LLBC_ListenerStub &stub);

    /**
     * Remove event deleg using deleg stub and clear the deleg stub.
     * @param[in] stub - event deleg stub.
     * @return int - success if return LLBC_OK, otherwise return LLBC_FAILED,
     *               specially, if return LLBC_FAILED, and fetch the last error is pending,
     *               it means operation will success on later, but pending at now.
     */
    virtual int RemoveListenerX(LLBC_ListenerStub &stub);

public:
    /**
     * Fire the event.
     * @param[in] event - event object.
     */
    virtual void FireEvent(LLBC_Event *event);

    /**
     * Fire the event.
     * @param[in] id - event Id.
     */
    void FireEvent(int id);

    /**
     * Check event manager is firing or not.
     * @return bool - firing flag.
     */
    virtual bool IsFiring() const;

protected:
    /**
     * Check given listen stub in the event manager exist or not.
     */
    bool HasStub(const LLBC_ListenerStub &stub) const;

    /**
     * Before fire event method.
     */
    void BeforeFireEvent();

    /**
     * After fire event method.
     */
    void AfterFireEvent();

protected:
    /**
     * \brief The event deleg info encapsulation.
     */
    struct _ListenerInfo
    {
        int evId;
        LLBC_ListenerStub stub;
        LLBC_EventListener *listener;
        LLBC_NewDelegate<void(LLBC_Event &)> deleg;

        _ListenerInfo();
        ~_ListenerInfo();
    };

private:
    int AddListenerCheck(const LLBC_ListenerStub &boundStub, LLBC_ListenerStub &stub);
    void AddListenerInfo(_ListenerInfo *li);

protected:
    typedef std::list<_ListenerInfo *> _Listeners; // deleg info list
    typedef std::map<int, _Listeners> _Id2Listeners; // event id 2 listeners
    typedef std::map<LLBC_ListenerStub, std::pair<int, _Listeners::iterator> > _Stub2Listeners; // stub id 2 listeners

    int _firing;
    LLBC_ListenerStub _maxListenerStub;

    _Id2Listeners _id2Listeners;
    _Stub2Listeners _stub2Listeners;

    std::set<int> _pendingRemoveEvIds;
    std::set<LLBC_ListenerStub> _pendingRemoveEvStubs;
};

__LLBC_NS_END

#include "llbc/core/event/EventManagerImpl.h"

#endif // !__LLBC_CORE_EVENT_EVENT_MANAGER_H__
