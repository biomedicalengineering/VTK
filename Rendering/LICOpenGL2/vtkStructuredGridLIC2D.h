/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkStructuredGridLIC2D.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkStructuredGridLIC2D
 *
 * GPU implementation of a Line Integral Convolution, a technique for
 * imaging vector fields.
 *
 * The input on port 0 is an 2D vtkStructuredGrid. It needs
 * a vector field on point data.
 * Port 1 is a special port for customized noise input. It is an optional port.
 * If not present, noise is generated by the filter. Even if none-power-of-two
 * texture are supported, giving a power-of-two image may result in faster
 * execution on the GPU.
 *
 * Please refer to Forssell, L. K., "Visualizing flow over curvilinear grid
 * surfaces using line integral convolution", Visualization 94 Conference
 * Proceedings, pages 240-247, IEEE Computer Society, 1994 for details of the
 * algorithm.
 *
 * @par Required OpenGL Extensions:
 * GL_ARB_texture_non_power_of_two
 * GL_VERSION_2_0
 * GL_ARB_texture_float
 * GL_ARB_draw_buffers
 * GL_EXT_framebuffer_object
 * GL_ARB_pixel_buffer_object
 */

#ifndef vtkStructuredGridLIC2D_h
#define vtkStructuredGridLIC2D_h

#include "vtkRenderingLICOpenGL2Module.h" // For export macro
#include "vtkStructuredGridAlgorithm.h"
#include "vtkWeakPointer.h" // needed for vtkWeakPointer.

class vtkImageNoiseSource;
class vtkOpenGLHelper;
class vtkRenderWindow;

class VTKRENDERINGLICOPENGL2_EXPORT vtkStructuredGridLIC2D : public vtkStructuredGridAlgorithm
{
public:
  static vtkStructuredGridLIC2D* New();
  vtkTypeMacro(vtkStructuredGridLIC2D, vtkStructuredGridAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Get/Set the context. Context must be a vtkOpenGLRenderWindow.
   * This does not increase the reference count of the
   * context to avoid reference loops.
   * SetContext() may raise an error is the OpenGL context does not support the
   * required OpenGL extensions. Return 0 upon failure and 1 upon success.
   */
  int SetContext(vtkRenderWindow* context);
  vtkRenderWindow* GetContext();
  //@}

  //@{
  /**
   * Number of steps. Initial value is 1.
   * class invariant: Steps>0.
   * In term of visual quality, the greater the better.
   */
  vtkSetMacro(Steps, int);
  vtkGetMacro(Steps, int);
  //@}

  //@{
  /**
   * Step size.
   * WE ARE NOT SURE YET about the space where we define the step.
   * If the image data has different spacing in each dimension, it
   * is an issue.
   * Initial value is 1.0.
   * class invariant: StepSize>0.0.
   * In term of visual quality, the smaller the better.
   * The type for the interface is double as VTK interface is double
   * but GPU only supports float. This value will be converted to
   * float in the execution of the algorithm.
   */
  vtkSetMacro(StepSize, double);
  vtkGetMacro(StepSize, double);
  //@}

  //@{
  /**
   * The magnification factor. Default is 1
   */
  vtkSetClampMacro(Magnification, int, 1, VTK_INT_MAX);
  vtkGetMacro(Magnification, int);
  //@}

  /**
   * Check if FBO is started properly.
   */
  int GetFBOSuccess() { return this->FBOSuccess; }

  /**
   * Check if LIC runs properly.
   */
  int GetLICSuccess() { return this->LICSuccess; }

protected:
  vtkStructuredGridLIC2D();
  ~vtkStructuredGridLIC2D() override;

  /**
   * Fill the input port information objects for this algorithm.  This
   * is invoked by the first call to GetInputPortInformation for each
   * port so subclasses can specify what they can handle.
   * Redefined from the superclass.
   */
  int FillInputPortInformation(int port, vtkInformation* info) override;

  /**
   * Fill the output port information objects for this algorithm.
   * This is invoked by the first call to GetOutputPortInformation for
   * each port so subclasses can specify what they can handle.
   * Redefined from the superclass.
   */
  int FillOutputPortInformation(int port, vtkInformation* info) override;

  int RequestInformation(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) override;

  int RequestUpdateExtent(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) override;

  /**
   * Stolen from vtkImageAlgorithm. Should be in vtkStructuredGridAlgorithm.
   */
  void AllocateOutputData(vtkDataObject* output, vtkInformation* outInfo);

  /**
   * Stolen from vtkImageData. Should be in vtkStructuredGrid.
   */
  void AllocateScalars(vtkStructuredGrid* sg, vtkInformation* outInfo);

  /**
   * This is called by the superclass.
   * This is the method you should override.
   */
  int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) override;

  int Steps;
  double StepSize;
  int Magnification;
  vtkWeakPointer<vtkRenderWindow> Context;

  vtkImageNoiseSource* NoiseSource;
  bool OwnWindow;
  int FBOSuccess;
  int LICSuccess;
  vtkOpenGLHelper* LICProgram;

private:
  vtkStructuredGridLIC2D(const vtkStructuredGridLIC2D&) = delete;
  void operator=(const vtkStructuredGridLIC2D&) = delete;
};

#endif
