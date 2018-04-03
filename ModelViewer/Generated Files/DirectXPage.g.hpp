﻿//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"

#pragma warning(push)
#pragma warning(disable: 4100) // unreferenced formal parameter

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BINDING_DEBUG_OUTPUT
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent();
#endif

#include "DirectXPage.xaml.h"

void ::ModelViewer::DirectXPage::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///DirectXPage.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}


/// <summary>
/// Auto generated class for compiled bindings.
/// </summary>
class ModelViewer::DirectXPage::DirectXPage_obj1_Bindings 
    : public ::XamlBindingInfo::ReferenceTypeXamlBindings<::ModelViewer::DirectXPage, ::XamlBindingInfo::XamlBindingTrackingBase>
    , public ::XamlBindingInfo::IXamlBindingTracking
{
public:
    DirectXPage_obj1_Bindings()
    {
        InitializeTracking(this);
    }

private:
    ~DirectXPage_obj1_Bindings()
    {
        ReleaseAllListeners();
    }

public:

    void Connect(int __connectionId, ::Platform::Object^ __target)
    {
        switch(__connectionId)
        {
            case 7: // DirectXPage.xaml line 133
                this->obj7 = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
                this->obj7->RegisterPropertyChangedCallback(::Windows::UI::Xaml::Controls::Primitives::RangeBase::ValueProperty,
                    ref new ::Windows::UI::Xaml::DependencyPropertyChangedCallback( 
                        [this] (::Windows::UI::Xaml::DependencyObject^ sender, ::Windows::UI::Xaml::DependencyProperty^ prop)
                        {
                            if (this->IsInitialized())
                            {
                                // Update Two Way binding
                                this->GetDataRoot()->ViewModel->LightScale = safe_cast<::default::float32>(this->obj7->Value);
                            }
                        }));
                break;
            case 8: // DirectXPage.xaml line 135
                this->obj8 = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
                this->obj8->RegisterPropertyChangedCallback(::Windows::UI::Xaml::Controls::Primitives::RangeBase::ValueProperty,
                    ref new ::Windows::UI::Xaml::DependencyPropertyChangedCallback( 
                        [this] (::Windows::UI::Xaml::DependencyObject^ sender, ::Windows::UI::Xaml::DependencyProperty^ prop)
                        {
                            if (this->IsInitialized())
                            {
                                // Update Two Way binding
                                this->GetDataRoot()->ViewModel->LightRotation = safe_cast<::default::float32>(this->obj8->Value);
                            }
                        }));
                break;
            case 9: // DirectXPage.xaml line 137
                this->obj9 = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
                this->obj9->RegisterPropertyChangedCallback(::Windows::UI::Xaml::Controls::Primitives::RangeBase::ValueProperty,
                    ref new ::Windows::UI::Xaml::DependencyPropertyChangedCallback( 
                        [this] (::Windows::UI::Xaml::DependencyObject^ sender, ::Windows::UI::Xaml::DependencyProperty^ prop)
                        {
                            if (this->IsInitialized())
                            {
                                // Update Two Way binding
                                this->GetDataRoot()->ViewModel->LightPitch = safe_cast<::default::float32>(this->obj9->Value);
                            }
                        }));
                break;
            case 10: // DirectXPage.xaml line 139
                this->obj10 = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(__target);
                this->obj10->RegisterPropertyChangedCallback(::Windows::UI::Xaml::Controls::Primitives::RangeBase::ValueProperty,
                    ref new ::Windows::UI::Xaml::DependencyPropertyChangedCallback( 
                        [this] (::Windows::UI::Xaml::DependencyObject^ sender, ::Windows::UI::Xaml::DependencyProperty^ prop)
                        {
                            if (this->IsInitialized())
                            {
                                // Update Two Way binding
                                this->GetDataRoot()->ViewModel->Ibl = safe_cast<::default::float32>(this->obj10->Value);
                            }
                        }));
                break;
            case 11: // DirectXPage.xaml line 154
                this->obj11 = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
                this->obj11->RegisterPropertyChangedCallback(::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty,
                    ref new ::Windows::UI::Xaml::DependencyPropertyChangedCallback( 
                        [this] (::Windows::UI::Xaml::DependencyObject^ sender, ::Windows::UI::Xaml::DependencyProperty^ prop)
                        {
                            if (this->IsInitialized())
                            {
                                // Update Two Way binding
                                this->GetDataRoot()->ViewModel->BaseColour = this->obj11->IsChecked ? this->obj11->IsChecked->Value : false;
                            }
                        }));
                break;
            case 12: // DirectXPage.xaml line 156
                this->obj12 = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
                this->obj12->RegisterPropertyChangedCallback(::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty,
                    ref new ::Windows::UI::Xaml::DependencyPropertyChangedCallback( 
                        [this] (::Windows::UI::Xaml::DependencyObject^ sender, ::Windows::UI::Xaml::DependencyProperty^ prop)
                        {
                            if (this->IsInitialized())
                            {
                                // Update Two Way binding
                                this->GetDataRoot()->ViewModel->Metallic = this->obj12->IsChecked ? this->obj12->IsChecked->Value : false;
                            }
                        }));
                break;
            case 13: // DirectXPage.xaml line 158
                this->obj13 = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
                this->obj13->RegisterPropertyChangedCallback(::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty,
                    ref new ::Windows::UI::Xaml::DependencyPropertyChangedCallback( 
                        [this] (::Windows::UI::Xaml::DependencyObject^ sender, ::Windows::UI::Xaml::DependencyProperty^ prop)
                        {
                            if (this->IsInitialized())
                            {
                                // Update Two Way binding
                                this->GetDataRoot()->ViewModel->Roughness = this->obj13->IsChecked ? this->obj13->IsChecked->Value : false;
                            }
                        }));
                break;
            case 14: // DirectXPage.xaml line 161
                this->obj14 = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
                this->obj14->RegisterPropertyChangedCallback(::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty,
                    ref new ::Windows::UI::Xaml::DependencyPropertyChangedCallback( 
                        [this] (::Windows::UI::Xaml::DependencyObject^ sender, ::Windows::UI::Xaml::DependencyProperty^ prop)
                        {
                            if (this->IsInitialized())
                            {
                                // Update Two Way binding
                                this->GetDataRoot()->ViewModel->Diffuse = this->obj14->IsChecked ? this->obj14->IsChecked->Value : false;
                            }
                        }));
                break;
            case 15: // DirectXPage.xaml line 163
                this->obj15 = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
                this->obj15->RegisterPropertyChangedCallback(::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty,
                    ref new ::Windows::UI::Xaml::DependencyPropertyChangedCallback( 
                        [this] (::Windows::UI::Xaml::DependencyObject^ sender, ::Windows::UI::Xaml::DependencyProperty^ prop)
                        {
                            if (this->IsInitialized())
                            {
                                // Update Two Way binding
                                this->GetDataRoot()->ViewModel->Specular = this->obj15->IsChecked ? this->obj15->IsChecked->Value : false;
                            }
                        }));
                break;
            case 16: // DirectXPage.xaml line 165
                this->obj16 = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
                this->obj16->RegisterPropertyChangedCallback(::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty,
                    ref new ::Windows::UI::Xaml::DependencyPropertyChangedCallback( 
                        [this] (::Windows::UI::Xaml::DependencyObject^ sender, ::Windows::UI::Xaml::DependencyProperty^ prop)
                        {
                            if (this->IsInitialized())
                            {
                                // Update Two Way binding
                                this->GetDataRoot()->ViewModel->F = this->obj16->IsChecked ? this->obj16->IsChecked->Value : false;
                            }
                        }));
                break;
            case 17: // DirectXPage.xaml line 167
                this->obj17 = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
                this->obj17->RegisterPropertyChangedCallback(::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty,
                    ref new ::Windows::UI::Xaml::DependencyPropertyChangedCallback( 
                        [this] (::Windows::UI::Xaml::DependencyObject^ sender, ::Windows::UI::Xaml::DependencyProperty^ prop)
                        {
                            if (this->IsInitialized())
                            {
                                // Update Two Way binding
                                this->GetDataRoot()->ViewModel->G = this->obj17->IsChecked ? this->obj17->IsChecked->Value : false;
                            }
                        }));
                break;
            case 18: // DirectXPage.xaml line 169
                this->obj18 = safe_cast<::Windows::UI::Xaml::Controls::CheckBox^>(__target);
                this->obj18->RegisterPropertyChangedCallback(::Windows::UI::Xaml::Controls::Primitives::ToggleButton::IsCheckedProperty,
                    ref new ::Windows::UI::Xaml::DependencyPropertyChangedCallback( 
                        [this] (::Windows::UI::Xaml::DependencyObject^ sender, ::Windows::UI::Xaml::DependencyProperty^ prop)
                        {
                            if (this->IsInitialized())
                            {
                                // Update Two Way binding
                                this->GetDataRoot()->ViewModel->D = this->obj18->IsChecked ? this->obj18->IsChecked->Value : false;
                            }
                        }));
                break;
        }
    }

    void DisconnectUnloadedObject(int connectionId)
    {
        throw ref new ::Platform::InvalidArgumentException("No unloadable elements to disconnect.");
    }

private:
    // Fields for each control that has bindings.
    ::Windows::UI::Xaml::Controls::Slider^ obj7;
    ::Windows::UI::Xaml::Controls::Slider^ obj8;
    ::Windows::UI::Xaml::Controls::Slider^ obj9;
    ::Windows::UI::Xaml::Controls::Slider^ obj10;
    ::Windows::UI::Xaml::Controls::CheckBox^ obj11;
    ::Windows::UI::Xaml::Controls::CheckBox^ obj12;
    ::Windows::UI::Xaml::Controls::CheckBox^ obj13;
    ::Windows::UI::Xaml::Controls::CheckBox^ obj14;
    ::Windows::UI::Xaml::Controls::CheckBox^ obj15;
    ::Windows::UI::Xaml::Controls::CheckBox^ obj16;
    ::Windows::UI::Xaml::Controls::CheckBox^ obj17;
    ::Windows::UI::Xaml::Controls::CheckBox^ obj18;
    
    // Fields for binding tracking.
    ::Windows::UI::Xaml::Data::INotifyPropertyChanged^ cachePC_ViewModel = nullptr;
    ::Windows::Foundation::EventRegistrationToken tokenPC_ViewModel;

    // Update methods for each path node used in binding steps.
    void Update_(::ModelViewer::DirectXPage^ obj, int phase)
    {
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | DATA_CHANGED | (1 << 0))) != 0)
            {
                this->Update_ViewModel(obj->ViewModel, phase);
            }
        }
    }
    void Update_ViewModel(::ViewModels::DirectXPageViewModel^ obj, int phase)
    {
        this->_bindingsTracking->UpdatePropertyChangedListener(obj, &this->cachePC_ViewModel, &tokenPC_ViewModel);
        if (obj != nullptr)
        {
            if ((phase & (NOT_PHASED | DATA_CHANGED | (1 << 0))) != 0)
            {
                this->Update_ViewModel_LightScale(obj->LightScale, phase);
                this->Update_ViewModel_LightRotation(obj->LightRotation, phase);
                this->Update_ViewModel_LightPitch(obj->LightPitch, phase);
                this->Update_ViewModel_Ibl(obj->Ibl, phase);
                this->Update_ViewModel_BaseColour(obj->BaseColour, phase);
                this->Update_ViewModel_Metallic(obj->Metallic, phase);
                this->Update_ViewModel_Roughness(obj->Roughness, phase);
                this->Update_ViewModel_Diffuse(obj->Diffuse, phase);
                this->Update_ViewModel_Specular(obj->Specular, phase);
                this->Update_ViewModel_F(obj->F, phase);
                this->Update_ViewModel_G(obj->G, phase);
                this->Update_ViewModel_D(obj->D, phase);
            }
        }
    }
    void Update_ViewModel_LightScale(::default::float32 obj, int phase)
    {
        if((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // DirectXPage.xaml line 133
            Set_Windows_UI_Xaml_Controls_Primitives_RangeBase_Value(this->obj7, obj);
        }
    }
    void Update_ViewModel_LightRotation(::default::float32 obj, int phase)
    {
        if((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // DirectXPage.xaml line 135
            Set_Windows_UI_Xaml_Controls_Primitives_RangeBase_Value(this->obj8, obj);
        }
    }
    void Update_ViewModel_LightPitch(::default::float32 obj, int phase)
    {
        if((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // DirectXPage.xaml line 137
            Set_Windows_UI_Xaml_Controls_Primitives_RangeBase_Value(this->obj9, obj);
        }
    }
    void Update_ViewModel_Ibl(::default::float32 obj, int phase)
    {
        if((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // DirectXPage.xaml line 139
            Set_Windows_UI_Xaml_Controls_Primitives_RangeBase_Value(this->obj10, obj);
        }
    }
    void Update_ViewModel_BaseColour(::Platform::Boolean obj, int phase)
    {
        if((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // DirectXPage.xaml line 154
            Set_Windows_UI_Xaml_Controls_Primitives_ToggleButton_IsChecked(this->obj11, obj, nullptr);
        }
    }
    void Update_ViewModel_Metallic(::Platform::Boolean obj, int phase)
    {
        if((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // DirectXPage.xaml line 156
            Set_Windows_UI_Xaml_Controls_Primitives_ToggleButton_IsChecked(this->obj12, obj, nullptr);
        }
    }
    void Update_ViewModel_Roughness(::Platform::Boolean obj, int phase)
    {
        if((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // DirectXPage.xaml line 158
            Set_Windows_UI_Xaml_Controls_Primitives_ToggleButton_IsChecked(this->obj13, obj, nullptr);
        }
    }
    void Update_ViewModel_Diffuse(::Platform::Boolean obj, int phase)
    {
        if((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // DirectXPage.xaml line 161
            Set_Windows_UI_Xaml_Controls_Primitives_ToggleButton_IsChecked(this->obj14, obj, nullptr);
        }
    }
    void Update_ViewModel_Specular(::Platform::Boolean obj, int phase)
    {
        if((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // DirectXPage.xaml line 163
            Set_Windows_UI_Xaml_Controls_Primitives_ToggleButton_IsChecked(this->obj15, obj, nullptr);
        }
    }
    void Update_ViewModel_F(::Platform::Boolean obj, int phase)
    {
        if((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // DirectXPage.xaml line 165
            Set_Windows_UI_Xaml_Controls_Primitives_ToggleButton_IsChecked(this->obj16, obj, nullptr);
        }
    }
    void Update_ViewModel_G(::Platform::Boolean obj, int phase)
    {
        if((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // DirectXPage.xaml line 167
            Set_Windows_UI_Xaml_Controls_Primitives_ToggleButton_IsChecked(this->obj17, obj, nullptr);
        }
    }
    void Update_ViewModel_D(::Platform::Boolean obj, int phase)
    {
        if((phase & ((1 << 0) | NOT_PHASED | DATA_CHANGED)) != 0)
        {
            // DirectXPage.xaml line 169
            Set_Windows_UI_Xaml_Controls_Primitives_ToggleButton_IsChecked(this->obj18, obj, nullptr);
        }
    }

    virtual void ReleaseAllListeners() override
    {
        this->_bindingsTracking->UpdatePropertyChangedListener(nullptr, &this->cachePC_ViewModel, &tokenPC_ViewModel);
    }

    virtual void PropertyChanged(Platform::Object^ sender, ::Windows::UI::Xaml::Data::PropertyChangedEventArgs^ e) override
    {
        if (cachePC_ViewModel != nullptr && cachePC_ViewModel->Equals(sender))
        {
            ::Platform::String^ propName = e->PropertyName;
            ::ViewModels::DirectXPageViewModel^ obj = safe_cast<::ViewModels::DirectXPageViewModel^>(sender);
            if (propName == nullptr || propName->IsEmpty())
            {
                if (obj != nullptr)
                {
                    this->Update_ViewModel_LightScale(obj->LightScale, DATA_CHANGED);
                    this->Update_ViewModel_LightRotation(obj->LightRotation, DATA_CHANGED);
                    this->Update_ViewModel_LightPitch(obj->LightPitch, DATA_CHANGED);
                    this->Update_ViewModel_Ibl(obj->Ibl, DATA_CHANGED);
                    this->Update_ViewModel_BaseColour(obj->BaseColour, DATA_CHANGED);
                    this->Update_ViewModel_Metallic(obj->Metallic, DATA_CHANGED);
                    this->Update_ViewModel_Roughness(obj->Roughness, DATA_CHANGED);
                    this->Update_ViewModel_Diffuse(obj->Diffuse, DATA_CHANGED);
                    this->Update_ViewModel_Specular(obj->Specular, DATA_CHANGED);
                    this->Update_ViewModel_F(obj->F, DATA_CHANGED);
                    this->Update_ViewModel_G(obj->G, DATA_CHANGED);
                    this->Update_ViewModel_D(obj->D, DATA_CHANGED);
                }
            }
            else if (propName == "LightScale")
            {
                if (obj != nullptr)
                {
                    this->Update_ViewModel_LightScale(obj->LightScale, DATA_CHANGED);
                }
            }
            else if (propName == "LightRotation")
            {
                if (obj != nullptr)
                {
                    this->Update_ViewModel_LightRotation(obj->LightRotation, DATA_CHANGED);
                }
            }
            else if (propName == "LightPitch")
            {
                if (obj != nullptr)
                {
                    this->Update_ViewModel_LightPitch(obj->LightPitch, DATA_CHANGED);
                }
            }
            else if (propName == "Ibl")
            {
                if (obj != nullptr)
                {
                    this->Update_ViewModel_Ibl(obj->Ibl, DATA_CHANGED);
                }
            }
            else if (propName == "BaseColour")
            {
                if (obj != nullptr)
                {
                    this->Update_ViewModel_BaseColour(obj->BaseColour, DATA_CHANGED);
                }
            }
            else if (propName == "Metallic")
            {
                if (obj != nullptr)
                {
                    this->Update_ViewModel_Metallic(obj->Metallic, DATA_CHANGED);
                }
            }
            else if (propName == "Roughness")
            {
                if (obj != nullptr)
                {
                    this->Update_ViewModel_Roughness(obj->Roughness, DATA_CHANGED);
                }
            }
            else if (propName == "Diffuse")
            {
                if (obj != nullptr)
                {
                    this->Update_ViewModel_Diffuse(obj->Diffuse, DATA_CHANGED);
                }
            }
            else if (propName == "Specular")
            {
                if (obj != nullptr)
                {
                    this->Update_ViewModel_Specular(obj->Specular, DATA_CHANGED);
                }
            }
            else if (propName == "F")
            {
                if (obj != nullptr)
                {
                    this->Update_ViewModel_F(obj->F, DATA_CHANGED);
                }
            }
            else if (propName == "G")
            {
                if (obj != nullptr)
                {
                    this->Update_ViewModel_G(obj->G, DATA_CHANGED);
                }
            }
            else if (propName == "D")
            {
                if (obj != nullptr)
                {
                    this->Update_ViewModel_D(obj->D, DATA_CHANGED);
                }
            }
        }
    }

    void CollectionChanged(::Platform::Object^ sender, ::Windows::UI::Xaml::Interop::NotifyCollectionChangedEventArgs^ e)
    {
    }

    void VectorChanged(::Platform::Object^ sender, ::Windows::Foundation::Collections::IVectorChangedEventArgs^ e)
    {
    }

    void MapChanged(::Platform::Object^ sender, ::Windows::Foundation::Collections::IMapChangedEventArgs<::Platform::String^>^ e)
    {
    }

    void DependencyPropertyChanged(::Windows::UI::Xaml::DependencyObject^ sender, ::Windows::UI::Xaml::DependencyProperty^ prop)
    {
        if (sender != nullptr)
        {
        }
    }
    static void Set_Windows_UI_Xaml_Controls_Primitives_RangeBase_Value(::Windows::UI::Xaml::Controls::Primitives::RangeBase^ obj, ::default::float64 value)
    {
        obj->Value = value;
    }
    static void Set_Windows_UI_Xaml_Controls_Primitives_ToggleButton_IsChecked(::Windows::UI::Xaml::Controls::Primitives::ToggleButton^ obj, ::Platform::IBox<::Platform::Boolean>^ value, ::Platform::String^ targetNullValue)
    {
        if (value == nullptr && targetNullValue != nullptr)
        {
            value = (::Platform::IBox<::Platform::Boolean>^) ::Windows::UI::Xaml::Markup::XamlBindingHelper::ConvertValue(::Platform::IBox<::Platform::Boolean>::typeid, targetNullValue);
        }
        obj->IsChecked = value;
    }
};

void ::ModelViewer::DirectXPage::Connect(int __connectionId, ::Platform::Object^ __target)
{
    switch (__connectionId)
    {
    case 2:
        {
            this->myColorPicker = safe_cast<::Windows::UI::Xaml::Controls::ColorPicker^>(__target);
        }
        break;
    case 3:
        {
            ::Windows::UI::Xaml::Controls::Button^ element3 = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Button^>(element3))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::ModelViewer::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::confirmColor_Click);
        }
        break;
    case 4:
        {
            ::Windows::UI::Xaml::Controls::Button^ element4 = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
            (safe_cast<::Windows::UI::Xaml::Controls::Button^>(element4))->Click += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::ModelViewer::DirectXPage::*)
                (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&DirectXPage::cancelColor_Click);
        }
        break;
    case 5:
        {
            this->swapChainPanel = safe_cast<::Windows::UI::Xaml::Controls::SwapChainPanel^>(__target);
        }
        break;
    case 6:
        {
            this->sampleTreeView = safe_cast<::TreeViewControl::TreeView^>(__target);
        }
        break;
    case 19:
        {
            this->colorPickerButton = safe_cast<::Windows::UI::Xaml::Controls::Button^>(__target);
        }
        break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::ModelViewer::DirectXPage::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    ::XamlBindingInfo::XamlBindings^ bindings = nullptr;
    switch (__connectionId)
    {
        case 1: // DirectXPage.xaml line 1
            {
                ::Windows::UI::Xaml::Controls::Page^ element1 = safe_cast<::Windows::UI::Xaml::Controls::Page^>(__target);
                DirectXPage_obj1_Bindings* objBindings = new DirectXPage_obj1_Bindings();
                objBindings->SetDataRoot(this);
                bindings = ref new ::XamlBindingInfo::XamlBindings(objBindings);
                this->Bindings = bindings;
                element1->Loading += ref new ::Windows::Foundation::TypedEventHandler<::Windows::UI::Xaml::FrameworkElement^, ::Platform::Object^>(bindings, &::XamlBindingInfo::XamlBindings::Loading);
            }
            break;
    }
    return bindings;
}

#pragma warning(pop)


