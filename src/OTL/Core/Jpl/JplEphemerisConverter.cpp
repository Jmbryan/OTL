////////////////////////////////////////////////////////////
//
// OTL - Orbital Trajectory Library
// Copyright (C) 2013-2018 Jason Bryan (Jmbryan10@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#include <OTL/Core/Jpl/JplEphemerisConverter.h>
#include <OTL/Core/Epoch.h>
#include <niek-ephem/DE405Ephemeris.h>
#include <fstream>

namespace otl
{
   typedef std::map<std::string, DE405Ephemeris::AstroEntity> AstroEntityDictionary;
   static AstroEntityDictionary g_entityDictionary;


   // Sub-class template of the std::num_get facet class template
   // used to parse numeric input character sequences
   template < class charT, class InputIterator = std::istreambuf_iterator<charT> >
   class DFormatNumGet : public std::num_get< charT, InputIterator > {

   public:
      // Ensure we have base types available
      typedef typename std::num_get< charT, InputIterator > base_type;
      typedef typename base_type::iter_type iter_type;
      typedef typename base_type::char_type char_type;

      // Replicate the base constructor. Pass on most of the work to the base 
      // constructor. Get and save reference to current global num_get facet
      DFormatNumGet(size_t refs = 0)
         : base_type(refs),
         iPrevNumGet(std::use_facet<base_type>(std::locale())) {
      }

   protected:
      // Override the base class virtual member function that handles 
      // parsing doubles type character sequences
      virtual iter_type do_get(iter_type in, iter_type end,
                               std::ios_base& ib,
                               std::ios_base::iostate& err,
                               double& v) const;

   private:
      // Reference to global num_get facet in effect when instance created
      base_type const &     iPrevNumGet;
   };


   // Implementation of overriden double value parsing function
   template < class charT, class InputIterator >
   typename DFormatNumGet<charT, InputIterator>::iter_type
      DFormatNumGet<charT, InputIterator>::do_get(iter_type in, iter_type end,
                                                  std::ios_base& ib,
                                                  std::ios_base::iostate& err,
                                                  double& v) const
   {
      // Initially parse sequence using previous num_get facet
      iter_type next(iPrevNumGet.get(in, end, ib, err, v));

      // Continue if there was no error
      if (0 == err || std::ios::goodbit == err) {

         // If we are not at the end of the sequence and the current character is
         // the D format exponent character then we have some work to do...
         if (next != end && *next == 'D') {

            // skip exponent character
            ++next;

            // Get the exponent value: This is a integer value so we use the 
            // stashed num_get facet reference to parse the integer value as a 
            // long:
            err = std::ios::goodbit;
            long exponent;
            next = iPrevNumGet.get(in, end, ib, err, exponent);

            if (std::ios::goodbit == err) {

               // If all well check exponent value is in range.  
               if (std::numeric_limits<double>::min_exponent10 <= exponent &&
                   std::numeric_limits<double>::max_exponent10 >= exponent)
               {
                  // If the exponent value is in range apply it to the double 
                  // value we already obtained:
                  v *= pow(10.0, exponent);
               }
               // exponent out of range 
               else
               {
                  err = std::ios::failbit;
               }
            }
         }
      }

      // next character position in sequence
      return next;
   }

   ////////////////////////////////////////////////////////////
   JplEphemerisConverter::JplEphemerisConverter() :
   m_initialized(false)
   {
      m_entityList = { "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn",
         "Uranus", "Neptune", "Pluto", "Sun", "Moon" };
   }

   ////////////////////////////////////////////////////////////
   JplEphemerisConverter::JplEphemerisConverter(const std::string& dataDirectory) :
   JplEphemerisConverter()
   {
      m_dataDirectory = dataDirectory;
   }

   ////////////////////////////////////////////////////////////
   void JplEphemerisConverter::SetDataDirectory(const std::string& dataDirectory)
   {
      m_dataDirectory = dataDirectory;
   }

   ////////////////////////////////////////////////////////////
   void JplEphemerisConverter::SetEntityList(const std::vector<std::string>& entityList)
   {
      m_entityList = entityList;
   }

   ////////////////////////////////////////////////////////////
   void JplEphemerisConverter::Initialize()
   {
      g_entityDictionary["Mercury"] = DE405Ephemeris::Mercury;
      g_entityDictionary["Venus"] = DE405Ephemeris::Venus;
      g_entityDictionary["Earth"] = DE405Ephemeris::EarthMoonBarycenter;
      g_entityDictionary["Mars"] = DE405Ephemeris::Mars;
      g_entityDictionary["Jupiter"] = DE405Ephemeris::JupiterBarycenter;
      g_entityDictionary["Saturn"] = DE405Ephemeris::SaturnBarycenter;
      g_entityDictionary["Uranus"] = DE405Ephemeris::UranusBarycenter;
      g_entityDictionary["Neptune"] = DE405Ephemeris::NeptuneBarycenter;
      g_entityDictionary["Pluto"] = DE405Ephemeris::PlutoBarycenter;
      g_entityDictionary["Sun"] = DE405Ephemeris::Sun;
      g_entityDictionary["Moon"] = DE405Ephemeris::Moon;

      m_initialized = true;
   }

   void JplEphemerisConverter::CreateFile(const Epoch& startDate, const Epoch& endDate, const std::string& outputFilename)
   {
      if (!m_initialized)
      {
         Initialize();
      }

      // Determine which entities to include ephemeris info for
      std::vector<bool> includeEntity(13, false);
      bool includedAtLeastOneEntity = false;
      for (const auto& entityName : m_entityList)
      {
         auto it = g_entityDictionary.find(entityName);
         if (it != g_entityDictionary.end())
         {
            includeEntity[it->second] = true;
            includedAtLeastOneEntity = true;
         }
      }
      if (!includedAtLeastOneEntity)
      {
         OTL_ERROR() << "No entities selected. No ephemeris file will be created";
         return;
      }

      // Extract the start and end days
      double startDay = startDate.GetJD();
      double endDay = endDate.GetJD();     
      if (startDay >= endDay)
      {
         OTL_ERROR() << "Start day must be less than end day";
         return;
      }

      int startYear = startDate.GetGregorian().year;
      int endYear = endDate.GetGregorian().year;
      if (startYear < 1600 || endYear > 2200)
      {
         OTL_ERROR() << "There is no ephemeris data for the dates selected. Must be between the years 1600..2200";
         return;
      }

      // Hardcoded data (could theoretically be read from header.405)
      int polyDegree[] =      
         { 14, 10,  13,  11,  8,   7,   6,   6,   6,   13,  11,  10,  10 };
      int numSubdivisions[] = 
         { 4,  2,   2,   1,   1,   1,   1,   1,   1,   8,   2,   4,   4 };
      int numDims[] = 
         { 3,  3,   3,   3,   3,   3,   3,   3,   3,   3,   3,   2,   3 };

      // Compute coefficient offsets and total
      int coeffPerRecord = 0;
      int coeffOffsets[13];
      for (int i = 0; i < 13; i++) {
         if (includeEntity[i]) {
            int numCoeffs = polyDegree[i] * numSubdivisions[i] * numDims[i];
            coeffOffsets[i] = coeffPerRecord;
            coeffPerRecord += numCoeffs;
         }
         else {
            coeffOffsets[i] = -1;
         }
      }

      // Open binary output file for results 
      std::ofstream ofs(outputFilename.c_str(), std::ios::binary | std::ios::trunc);
      if (!ofs)
      {
         OTL_ERROR() << "Failed to open output file " << Bracket(outputFilename);
         return;
      }

      // Dump the layout information
      for (int i = 0; i < 13; i++)
         ofs.write(reinterpret_cast<char*>(coeffOffsets + i), sizeof(int));
      for (int i = 0; i < 13; i++)
         ofs.write(reinterpret_cast<char*>(polyDegree + i), sizeof(int));
      for (int i = 0; i < 13; i++)
         ofs.write(reinterpret_cast<char*>(numSubdivisions + i), sizeof(int));

      // Dump the number of coeffs per record
      ofs.write(reinterpret_cast<char*>(&coeffPerRecord), sizeof(int));

      // Dump the start/stop day
      ofs.write(reinterpret_cast<char*>(&startDay), sizeof(double));
      ofs.write(reinterpret_cast<char*>(&endDay), sizeof(double));

      // Create translator facet handling FORTRAN doubles with 'D' instead of 'E'
      DFormatNumGet<char> dFormatNumGet(1);

      // Grab a copy of the global locale and replace in our translator facet
      std::locale dFormatLocale(std::locale(), &dFormatNumGet);

      // Open the ASCII file associated with each year, dump out coeffs
      int bufI;
      double bufD;

      // track total number of records written to binary
      int totalRecords = 0;

      // records may be duplicated across 20 yr blocks
      double lastWrittenStart = -1.0;

      int fstart = Max(1600, startYear - (startYear % 20));
      int fend = Min(2200, endYear + 1 - ((endYear + 1) % 20));

      for (int fnum = fstart; fnum < fend; fnum += 20) {

         // Forge the filename
         std::ostringstream fnameStr;
         fnameStr << m_dataDirectory << "\\ascp" << fnum << ".405";

         // Open the ASCII data file
         std::ifstream ifs(fnameStr.str().c_str());
         if (!ifs)
         {
            OTL_ERROR() << "Failed to open input file " << Bracket(fnameStr.str());
            return;
         }

         // Imbue stream with our FORTRAN exponent translator
         ifs.imbue(dFormatLocale);

         // Pull every record, failure to get record number implies eof
         int recordsWritten = 0;
         while (ifs >> bufI) {

            // Read and ignore num coeffs
            ifs >> bufI;

            // Read start/end day at beginning of record
            double currentStart;
            ifs >> currentStart >> bufD;

            // Read coeffs and write to binary output file 
            for (int i = 0; i < 1016; i++) {

               // Suck in ascii coeff value
               ifs >> bufD;

               // Check if we are writing record (not dup) and this 
               // entity is enabled
               int tmp = i;
               int curEnt;
               for (curEnt = 0; curEnt < 13; curEnt++)
               {
                  tmp -= polyDegree[curEnt] * numSubdivisions[curEnt] * numDims[curEnt];
                  if (tmp < 0)
                  {
                     break;
                  }
               }

               if ((currentStart > lastWrittenStart) &&
                   (currentStart >= startDay) &&
                   (currentStart <= endDay) &&
                   (includeEntity[curEnt]))
               {
                   ofs.write(reinterpret_cast<char*>(&bufD), sizeof(double));
               }
            }

            // Last two vals in coeff block always 0.0 and should be discarded
            ifs >> bufD >> bufD;

            if ((currentStart > lastWrittenStart) &&
                (currentStart >= startDay) &&
                (currentStart <= endDay))
            {
               lastWrittenStart = currentStart;
               recordsWritten++;
            }

         }
         
         OTL_INFO() << "Wrote " << recordsWritten << " records from " << Bracket(fnameStr.str());
         totalRecords += recordsWritten;
      }

      OTL_INFO() << "Total of " << totalRecords << " records written to " << Bracket(outputFilename);
   }

} // namespace otl