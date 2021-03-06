/*
Project Name : OpenMEEG

© INRIA and ENPC (contributors: Geoffray ADDE, Maureen CLERC, Alexandre 
GRAMFORT, Renaud KERIVEN, Jan KYBIC, Perrine LANDREAU, Théodore PAPADOPOULO,
Emmanuel OLIVI
Maureen.Clerc.AT.sophia.inria.fr, keriven.AT.certis.enpc.fr,
kybic.AT.fel.cvut.cz, papadop.AT.sophia.inria.fr)

The OpenMEEG software is a C++ package for solving the forward/inverse
problems of electroencephalography and magnetoencephalography.

This software is governed by the CeCILL-B license under French law and
abiding by the rules of distribution of free software.  You can  use,
modify and/ or redistribute the software under the terms of the CeCILL-B
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's authors,  the holders of the
economic rights,  and the successive licensors  have only  limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and,  more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-B license and that you accept its terms.
*/

#pragma once

#include <string>
#include <map>
#include <fstream>

#include <IOUtils.H>
#include <DataTag.H>

namespace Utils {

    namespace Properties {

        template <typename PROPERTIES>
        struct PropertyLoader {

            PropertyLoader(PROPERTIES& prop): filename("[unkmown]"),properties(prop) { }

            PropertyLoader(const char* name,PROPERTIES& prop): filename(name),properties(prop) {
                std::ifstream ifs(filename);
                ifs >> *this;
            }

            const char* filename;
            PROPERTIES& properties;
        };

        template <typename PROPERTIES>
        std::istream& operator>>(std::istream& is,PropertyLoader<PROPERTIES>& loader) {

            typedef typename PROPERTIES::Id       Id;
            typedef typename PROPERTIES::Property Property;

            Types::DataTag<PROPERTIES> tag;

            try {
                is >> io_utils::match("# Properties Description 1.0 (") >> tag >> io_utils::match(")");
            } catch(...) {
                std::cerr << "Wrong property file: " << loader.filename << '.' << std::endl
                          << "  Check whether the file exists and contains the proper properties." << std::endl;
                exit(1);
            }

            while (is.peek()!=std::istream::traits_type::eof()) {

                Id       id;
                Property property;

                is >> io_utils::skip_comments("#") >> id >> property >> std::ws;
                try {
                    loader.properties.define(id,property);
                } catch(int& i) {
                    std::cerr << "Double definition of " << id << " in " << loader.filename << "( error " << i << ")." << std::endl
                              << "Keeping first definition...." << std::endl;
                }
            }
            return is;
        }

        //  A class that can associate various properties to identities.
        //  Can be used as a basis class for converting names into effective values.
        //  This is mainly to be used in the case where there is a small number of
        //  properties.

        template <typename ID,typename PROPERTY>
        class Named: public std::map<ID,PROPERTY> {
            
            typedef std::map<ID,PROPERTY> base;

        public:

            typedef ID       Id;
            typedef PROPERTY Property;

            Named(): base() { }

            Named(const char* filename): base() { PropertyLoader<Named> loader(filename,*this); }

            //  Define and retrieve a property using its id.

            const Property& find(const Id& id) const { return base::find(id)->second; }

            void define(const Id& id,const Property& prop) throw(char*) {
                if (base::find(id)!=base::end()) {
                    throw "Double definition";
                } else {
                    base::insert(typename base::value_type(id,prop));
                }
            }
        };

        template <typename ID,typename PROPERTY>
        std::istream& operator>>(std::istream& is,Named<ID,PROPERTY>& properties) {
            PropertyLoader<Named<ID,PROPERTY> > loader(properties);
            return is >> loader;
        }

        template <typename ID,typename PROPERTY>
        std::ostream& operator<<(std::ostream& os,const Named<ID,PROPERTY>& properties) {
            for (typename Named<ID,PROPERTY>::const_iterator i=properties.begin();i!=properties.end();++i)
                os << i->first << ' ' << i->second << std::endl;
            return os;
        }
    }
}
