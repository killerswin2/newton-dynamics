/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.2
 *
 * This file is not intended to be easily readable and contains a number of
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG
 * interface file instead.
 * ----------------------------------------------------------------------------- */

#ifndef SWIG_newton_WRAP_H_
#define SWIG_newton_WRAP_H_

class SwigDirector_ndBodyNotifyGlue : public ndBodyNotifyGlue, public Swig::Director {

public:
    void swig_connect_director(JNIEnv *jenv, jobject jself, jclass jcls, bool swig_mem_own, bool weak_global);
    SwigDirector_ndBodyNotifyGlue(JNIEnv *jenv);
    virtual ~SwigDirector_ndBodyNotifyGlue();
    virtual void OnTransform(ndMatrixGlue const &matrix);
    virtual void OnApplyExternalForce(ndFloat32 timestep);
public:
    bool swig_overrides(int n) {
      return (n < 2 ? swig_override[n] : false);
    }
protected:
    Swig::BoolArray<2> swig_override;
};


#endif
