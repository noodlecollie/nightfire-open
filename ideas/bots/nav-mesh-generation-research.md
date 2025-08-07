This research is gathered from the [leaked Source Engine code](https://github.com/TheAlePower/TeamFortress2) from April 2020.

# How the Source Engine Does Nav Generation

The interface to the Source Engine navigation mesh is defined within CNavMesh (`tf2_src/game/server/nav_mesh.h`). When generating, this acts as a big state machine that performs a certain amount of work on each frame.

When generating a nav mesh from scratch (not incrementally), the function call stack looks something like the following. This gives us a cross-section of the multitude of processing that is required in order to build the entire mesh.

Note that processing happens across frames, and is time-limited per frame. The function call tree stack below is simplified to remove the asynchronous details.

```
// 1. Beginning generation

CNavMesh::BeginGeneration()
    CNavMesh::m_generationState = SAMPLE_WALKABLE_SPACE
    CNavMesh::DestroyNavigationMesh()
    CNavMesh::BuildLadders()
    CNavMesh::AddWalkableSeeds()

// 2. Sampling walkable space

CNavMesh::UpdateGeneration()
    CNavMesh::SampleStep()
        CNavMesh::GetNextWalkableSeedNode()

        for each dir in (north, east, south, west):
            pos = CNavMesh::m_currentNode->pos + (dir * GenerationStepSize)
            CNavMesh::m_currentNode->MarkAsVisited(dir)

            CNavMesh::TraceAdjacentNode()
                UTIL_TraceHull()

            if TraceAdjacentNode() not successful:
                re-trace with manual height adjustments etc.
                if still not successful, return

            CNavMesh::AddNode()

    CNavMesh::m_generationState = CREATE_AREAS_FROM_SAMPLES

// 3. Creating areas from samples

CNavMesh::UpdateGeneration()
    CNavMesh::CreateNavAreasFromNodes()
        CNavMesh::TestArea()
        CNavMesh::BuildArea()
            CNavMesh::CreateArea()
            CNavArea::Build()

        CNavMesh::AllocateGrid()

        for each area built earlier:
            CNavMesh::AddNavArea(area)

        CNavMesh::ConnectGeneratedAreas()
            CNavMesh::StitchGeneratedAreas()

        CNavMesh::MarkPlayerClipAreas() (only applicable in L4D)
        CNavMesh::MarkJumpAreas()
        CNavMesh::MergeGeneratedAreas()
        CNavMesh::SplitAreasUnderOverhangs()
        CNavMesh::SquareUpAreas()

        CNavMesh::MarkStairAreas()
            CNavArea::TestStairs()
                IsStairs()

        CNavMesh::StichAndRemoveJumpAreas()
            for each jump area:
                JumpConnector::operator ()()

        CNavMesh::HandleObstacleTopAreas()
            CNavMesh::RaiseAreasWithInternalObstacles()
            CNavMesh::CreateObstacleTopAreas()
                CNavMesh::CreateObstacleTopAreaIfNecessary()
            CNavMesh::RemoveOverlappingObstacleTopAreas()

        CNavMesh::FixUpGeneratedAreas()
            CNavMesh::FixCornerOnCornerAreas();
            CNavMesh::FixConnections();

        for each ladder:
            CNavLadder::ConnectGeneratedLadder()

    CNavMesh::DestroyHidingSpots()
    CNavMesh::m_generationState = FIND_HIDING_SPOTS

// 4. Finding hiding spots

CNavMesh::UpdateGeneration()
    for each nav area:
        CNavArea::ComputeHidingSpots()

    CNavMesh::m_generationState = FIND_ENCOUNTER_SPOTS

// 5. Finding encounter spots

CNavMesh::UpdateGeneration()
    for each nav area:
        CNavArea::ComputeSpotEncounters()
            CNavArea::AddSpotEncounters()

    CNavMesh::m_generationState = FIND_SNIPER_SPOTS

// 6. Finding sniper spots

CNavMesh::UpdateGeneration()
    for each nav area:
        CNavArea::ComputeSniperSpots()
            CNavArea::ClassifySniperSpot()

    CNavMesh::m_generationState = COMPUTE_MESH_VISIBILITY

    CNavMesh::BeginVisibilityComputations()
        for each nav area:
            CNavArea::ResetPotentiallyVisibleAreas()

// 7. Computing mesh visibility

CNavMesh::UpdateGeneration()
    for each nav area:
        CNavArea::ComputeVisibilityToMesh()
            for each nav area in nav_max_view_distance radius:
                NavAreaCollector::operator ()()

            CNavArea::SetupPVS()

            call CNavArea::ComputeVisToArea in parallel for each area in radius:
                CNavArea::ComputeVisibility()

    CNavMesh::EndVisibilityComputations()
    CNavMesh::::m_generationState = FIND_EARLIEST_OCCUPY_TIMES

// 8a. Finding earliest occupy times

CNavMesh::UpdateGeneration()
    for each nav area:
        CNavMesh::ComputeEarliestOccupyTimes() // (only applicable in CS)
            NavAreaTravelDistance()

    if step for finding light intensity is enabled:
        CNavMesh::m_generationState = FIND_LIGHT_INTENSITY
        CNavArea::MakeNewMarker()
    else:
        CNavMesh::m_generationState = CUSTOM

// 8b. Finding light intensity

CNavMesh::UpdateGeneration()
    for each nav area:
        CNavArea::ComputeLighting()

    CNavMesh::m_generationState = CUSTOM

// 9. Custom game-specific analysis

CNavMesh::UpdateGeneration()
    CNavMesh::BeginCustomAnalysis()

    for each nav area:
        CNavArea::CustomAnalysis()

    CNavMesh::PostCustomAnalysis()
    CNavMesh::EndCustomAnalysis()
    CNavMesh::m_generationState = SAVE_NAV_MESH

// 10. Saving mesh

CNavMesh::UpdateGeneration()
    CNavMesh::ClearWalkableSeeds()
    CNavMesh::HideAnalysisProgress()
    CNavMesh::Save()
        ??? No implementation available
```
