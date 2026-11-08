#include "tutorial_stage_map_files.hpp"
namespace ttyd {
FieldMapFileCatalog tutorialStageMapFileCatalog(){
 auto rows=FieldMapFileOwner::originalHarborCatalog();
 // Original FST logical bytes and actual rounded32-byte DVD transfer sizes.
 rows.push_back({"./m/bti_01/d","maps/tutorial-stage/bti_01/d",181042,181056});
 rows.push_back({"./m/bti_01/t","maps/tutorial-stage/bti_01/t",58976,58976});
 rows.push_back({"./m/stg_00_0/d","maps/tutorial-stage/stg_00_0/d",129290,129312});
 rows.push_back({"./m/stg_00_0/t","maps/tutorial-stage/stg_00_0/t",119168,119168});
 return rows;
}
}
