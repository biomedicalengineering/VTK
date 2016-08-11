vtk_module(vtkIOVPIC
  TEST_DEPENDS
    vtkRendering${VTK_RENDERING_BACKEND}
    vtkTestingRendering
    vtkInteractionStyle
  KIT
    vtkParallel
  DEPENDS
    vtkCommonExecutionModel
    vtkVPIC
  PRIVATE_DEPENDS
    vtkCommonCore
    vtkCommonDataModel
    vtkCommonMisc
    vtkParallelCore
  )