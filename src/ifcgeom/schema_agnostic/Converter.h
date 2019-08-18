#ifndef ITERATOR_KERNEL_H
#define ITERATOR_KERNEL_H

#include "../../ifcparse/IfcFile.h"
#include "../../ifcgeom/settings.h"
#include "../../ifcgeom/schema_agnostic/ConversionResult.h"
#include "../../ifcgeom/abstract_mapping.h"
#include "../../ifcgeom/kernel_agnostic/AbstractKernel.h"

#include <boost/function.hpp>

namespace ifcopenshell { namespace geometry {

	class NativeElement;

	class Converter {
	private:
		abstract_mapping* mapping_;
		kernels::AbstractKernel* kernel_;

	public:
		// Tolerances and settings for various geometrical operations:
		enum GeomValue {
			// Specifies the deflection of the mesher
			// Default: 0.001m / 1mm
			GV_DEFLECTION_TOLERANCE,
			// Specifies the minimal area of a face to be included in an IfcConnectedFaceset
			// Read-only
			GV_MINIMAL_FACE_AREA,
			// Specifies the threshold distance under which cartesian points are deemed equal
			// Read-only
			GV_POINT_EQUALITY_TOLERANCE,
			// Specifies maximum number of faces for a shell to be reoriented.
			// Default: -1
			GV_MAX_FACES_TO_ORIENT,
			// The length unit used the creation of TopoDS_Shapes, primarily affects the
			// interpretation of IfcCartesianPoints and IfcVector magnitudes
			// DefaultL 1.0
			GV_LENGTH_UNIT,
			// The plane angle unit used for the creation of TopoDS_Shapes, primarily affects
			// the interpretation of IfcParamaterValues of IfcTrimmedCurves
			// Default: -1.0 (= not set, fist try degrees, then radians)
			GV_PLANEANGLE_UNIT,
			// The precision used in boolean operations, setting this value too low results
			// in artefacts and potentially modelling failures
			// Default: 0.00001 (obtained from IfcGeometricRepresentationContext if available)
			GV_PRECISION,
			// Whether to process shapes of type Face or higher (1) Wire or lower (-1) or all (0)
			GV_DIMENSIONALITY
		};

		Converter(const std::string& geometry_library, IfcParse::IfcFile* file);
		
		~Converter() {}

		abstract_mapping* mapping() const { return mapping_; }

		/*
		virtual void setValue(GeomValue var, double value) {
			implementation_->setValue(var, value);
		}

		virtual double getValue(GeomValue var) const {
			return implementation_->getValue(var);
		}
		*/

		/*
		virtual NativeElement<double, double>* convert(
			const IteratorSettings& settings, IfcUtil::IfcBaseClass* representation,
			IfcUtil::IfcBaseClass* product)
		{
			return implementation_->convert(settings, representation, product);
		}
		*/

		ifcopenshell::geometry::ConversionResults convert(IfcUtil::IfcBaseClass* item) {
			auto geom_item = mapping_->map(item);
			ifcopenshell::geometry::ConversionResults results;
			kernel_->convert(geom_item, results);
			return results;
		}

		ifcopenshell::geometry::NativeElement* create_brep_for_representation_and_product(const ifcopenshell::geometry::settings& settings, IfcUtil::IfcBaseEntity* representation, IfcUtil::IfcBaseEntity* product);
		ifcopenshell::geometry::NativeElement* create_brep_for_processed_representation(const ifcopenshell::geometry::settings& settings, IfcUtil::IfcBaseEntity* representation, IfcUtil::IfcBaseEntity* product, ifcopenshell::geometry::NativeElement* brep);

		/*
		static int count(const ifcopenshell::geometry::ConversionResultShape*, int, bool unique=false);
		static int surface_genus(const ifcopenshell::geometry::ConversionResultShape*);

		static bool is_manifold(const ifcopenshell::geometry::ConversionResultShape*);
		static IfcUtil::IfcBaseEntity* get_decomposing_entity(IfcUtil::IfcBaseEntity*, bool include_openings=true);
		static IfcEntityList::ptr find_openings(IfcUtil::IfcBaseEntity* product);
		*/
	};
}}

#endif