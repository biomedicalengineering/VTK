vtk_module(vtkCommonDataModel
  GROUPS
    StandAlone
  TEST_DEPENDS
    vtkTestingCore
    vtkTestingRendering
    vtkInteractionStyle
    vtkCommonExecutionModel
    vtkFiltersGeneric
    vtkFiltersModeling
    vtkIOGeometry
    vtkIOLegacy
    vtkIOXML
    vtkTestingGenericBridge
    vtkRendering${VTK_RENDERING_BACKEND}
  KIT
    vtkCommon
  DEPENDS
    vtkCommonCore
    vtkCommonMath
    vtkCommonTransforms
  PRIVATE_DEPENDS
    vtkCommonMisc
    vtkCommonSystem
    vtksys
  )