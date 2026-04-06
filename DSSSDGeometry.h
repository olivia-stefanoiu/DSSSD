namespace DSSSDGeometry
{
    constexpr double R0 = 24.0;
    constexpr double R0_inter = 25.425;
    constexpr double H = 1.505;
    constexpr double Hactif = 1.425;
    constexpr double Hinter = H - Hactif;
    constexpr double Pi = 3.14159265358979323846;
    constexpr double SectorWidthRad =  2.0* Pi / 16.0;
    constexpr double QuadrantWidthRad = 2.0 * Pi / 4.0;
    constexpr double NumStripsQuadrant = 16;
    constexpr UShort_t num_dsssdStrips = 64;
    constexpr UShort_t num_dsssdSectors = 16;
}