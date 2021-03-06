#include "NativeObject.h"

namespace kiwano
{
NativeObjectBase::NativeObjectBase()
    : native_pointer_(nullptr)
{
}

bool NativeObjectBase::IsValid() const
{
    return native_pointer_ != nullptr;
}

void NativeObjectBase::ResetNativePointer(void* native_pointer)
{
    native_pointer_ = native_pointer;
}

//
// NativeObject for Windows
//
#if defined(KGE_PLATFORM_WINDOWS)

NativeObject::~NativeObject()
{
    ResetNativePointer();
}

void NativeObject::ResetNativePointer(void* native_pointer)
{
    if (native_pointer_)
    {
        static_cast<IUnknown*>(native_pointer_)->Release();
        native_pointer_ = nullptr;
    }

    if (native_pointer)
    {
        native_pointer_ = native_pointer;
        static_cast<IUnknown*>(native_pointer_)->AddRef();
    }
}

#endif

}
