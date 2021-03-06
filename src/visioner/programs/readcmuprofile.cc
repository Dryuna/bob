/**
 * @file visioner/programs/readcmuprofile.cc
 * @date Fri 27 Jul 13:58:57 2012 CEST
 * @author Andre Anjos <andre.anjos@idiap.ch>
 *
 * @brief This file was part of Visioner and originally authored by "Cosmin
 * Atanasoaei <cosmin.atanasoaei@idiap.ch>". It was only modified to conform to
 * Bob coding standards and structure.
 *
 * Copyright (C) 2011-2013 Idiap Research Institute, Martigny, Switzerland
 */

#include <boost/algorithm/string/split.hpp>

#include "bob/core/logging.h"

#include "bob/visioner/vision/object.h"

// Parse a .dat CMUProfile ground truth fileb
bool parse(const std::string& file) {
  // Load file content
  std::string text;
  if (bob::visioner::load_file(file, text) == false)
  {
    bob::core::error << "Failed to load <" << file << ">!" << std::endl;
    return false;
  }

  std::vector<std::string> lines;
  boost::split(lines, text, boost::is_any_of("\n"));
  for (uint64_t i = 0; i < lines.size(); i ++)
  {
    std::vector<std::string> tokens;
    boost::split(tokens, lines[i], boost::is_any_of("\t {}"));
    if (tokens.empty() == true)
    {
      continue;
    }

    const std::string ifile = tokens[0];
    const std::string gfile = bob::visioner::basename(ifile) + ".gt";

    bob::visioner::Object object("face", "unknown", "unknown");                
    for (uint64_t j = 0; 3 * j < tokens.size() - 3; j ++)
    {
      const std::string k = tokens[3 * j + 1];
      const std::string x = tokens[3 * j + 2];
      const std::string y = tokens[3 * j + 3];

      std::string new_k;
      if (k == "leye")
      {
        new_k = k;
      }
      else if (k == "reye")
      {
        new_k = k;
      }
      else if (k == "lmouth")
      {
        new_k = "lmc";
      }
      else if (k == "rmouth")
      {
        new_k = "rmc";
      }
      else if (k == "nose")
      {
        new_k = "nose";
      }
      else if (k == "chin")
      {
        new_k = "chin";
      }

      if (new_k.empty() == false)
      {                        
        object.add(bob::visioner::Keypoint(
              new_k, 
              boost::lexical_cast<float>(x),
              boost::lexical_cast<float>(y)));
      }
    }

    std::vector<bob::visioner::Object> objects;
    bob::visioner::Object::load(gfile, objects);

    objects.push_back(object);
    bob::visioner::Object::save(gfile, objects); 
  }

  // OK
  return true;
}

int main(int argc, char *argv[]) {	

  const std::string profile_dat = "testing_profile_ground_truth.dat";
  const std::string frontal_dat = "testing_frontal_ground_truth.dat";

  parse(profile_dat);
  parse(frontal_dat);

  // OK
  bob::core::info << "Program finished successfully" << std::endl;
  return EXIT_SUCCESS;
}
