#include "thirdperson.hpp"


void Thirdperson::OverrideView(CViewSetup& pSetup) {
    if(!set.misc.thirdperson) {
        pInput->m_fCameraInThirdPerson = false;
        pInput->m_vecCameraOffset.z = 150.0f;
        return;
    }
    
    float distance = 100.f;
    
    C_BaseEntity *localplayer = (C_BaseEntity*)pEntList->GetClientEntity(pEngine->GetLocalPlayer());
    if(!localplayer || !localplayer->IsAlive())
        return;
    
    Vector view = localplayer->GetViewAngles();
    trace_t tr;
    Ray_t ray;
    
    Vector desiredCamOffset = Vector(cos(DEG2RAD(view.y)) * distance, sin(DEG2RAD(view.y)) * distance, sin(DEG2RAD(-view.x)) * distance);
    
    ray.Init(localplayer->GetEyePosition(), (localplayer->GetEyePosition() - desiredCamOffset));
    CTraceFilter traceFilter;
    traceFilter.pSkip = localplayer;
    pEngineTrace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);
    
    Vector diff = localplayer->GetEyePosition() - tr.endpos;
    
    float distance2D = diff.Length2D();
    
    bool horOK = distance2D > (distance - 2.0f);
    bool vertOK = (abs(diff.z) - abs(desiredCamOffset.z) < 3.0f);
    
    float cameraDistance;
    
    if( horOK && vertOK )  // If we are clear of obstacles
    {
        cameraDistance= distance; // go ahead and set the distance to the setting
    }
    else
    {
        if( vertOK ) // if the Vertical Axis is OK
        {
            cameraDistance = distance2D * 0.95f;
        }
        else// otherwise we need to move closer to not go into the floor/ceiling
        {
            cameraDistance = abs(diff.z) * 0.95f;
        }
    }
    
    Vector origin; // Origin angles which we're seeing
    pEngine->GetViewAngles( origin );
    
    pInput->m_fCameraInThirdPerson = true;
    pInput->m_vecCameraOffset.x = origin.x;
    pInput->m_vecCameraOffset.y = origin.y;
    pInput->m_vecCameraOffset.z = cameraDistance;
}
