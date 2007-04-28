/*
 * =====================================================================================
 * 
 *       Filename:  hyper_ball_impl.h
 * 
 *    Description:  
 * 
 *        Version:  1.0
 *        Created:  02/11/2007 07:08:40 PM EST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  Nikolaos Vasiloglou (NV), nvasil@ieee.org
 *        Company:  Georgia Tech Fastlab-ESP Lab
 * 
 * =====================================================================================
 */

#ifndef HYPER_BALL_IMPL_H_
#define HYPER_BALL_IMPL_H_
#define __TEMPLATE__            \
 template<typename TYPELIST,  bool     diagnostic>
#define __HYPERBALL__           \
 HyperBall<TYPELIST, diagnostic>

__TEMPLATE__
__HYPERBALL__::HyperBall() {
}

__TEMPLATE__
void __HYPERBALL__::Init(int32 dimension) {
  center_.Reset(Allocator_t::calloc<Precision_t>(dimension, 0));
	pivot_left_.Reset(Allocator_t::calloc<Precision_t>(dimension, 0));
  pivot_right_.Reset(Allocator_t::calloc<Precision_t>(dimension, 0));
}

__TEMPLATE__
void __HYPERBALL__::Init(Array_t center, Precision_t radious, 
		                     Array_t pivot_left, Array_t pivot_right) {
  center_=center ;
  radious_=radious;
  pivot_left_=pivot_left;
  pivot_right_=pivot_right; 
}

__TEMPLATE__
static void *__HYPERBALL__::operator new(size_t size) {
  return Allocator_t::malloc(size);
}

__TEMPLATE__
static void  __HYPERBALL__::operator delete(void *p) {
}

__TEMPLATE__
inline HyperBall<TYPELIST, diagnostic> &__HYPERBALL__::operator=(
		const HyperBall<TYPELIST, diagnostic>  &other) {
  center_ = other.center_;
	radious_ = other.radious_;
	pivot_left_ = other.pivot_left_;
  pivot_right_ = other.pivot_right_;
	return *this;
}

__TEMPLATE__
inline void __HYPERBALL__::Alias(const HyperBall_t  &other) {
  center_ = other.center_;
	radious_ = other.radious_;
	pivot_left_ = other.pivot_left_;
  pivot_right_ = other.pivot_right_;
}

__TEMPLATE__
inline void __HYPERBALL__::Copy(const HyperBall_t  &other, 
		                                int32 dimension) {
  center_.Copy(other.center_, dimension);
	radious_ = other.radious_;
	pivot_left_.Copy(other.pivot_left_, dimension);
  pivot_right_.Copy(other.pivot_right_, dimension);
}



__TEMPLATE__
template<typename POINTTYPE> 
inline bool __HYPERBALL__::IsWithin(POINTTYPE point, 
		                                int32 dimension, 
				  													Precision_t range, 
                                    ComputationsCounter<diagnostic> &comp) {
  comp.UpdateDistances();
	Precision_t point_center_distance = 
		 Metric_t::Distance(center_, point, dimension);
  // Inside the ball
	comp.UpdateComparisons();
 	if (radious_ > sqrt(point_center_distance) + sqrt(range)) {
	  return true;
	} else {
		// Completelly outside or crossing
	  return false;
	}
	
}

__TEMPLATE__
inline Precision_t __HYPERBALL__::IsWithin(HyperBall_t &hr,
    int32 dimension, 
    Precision_t range,
    ComputationsCounter<diagnostic> &comp) {
  return IsWithin(hr.center_, dimension, hr.radious_ * hr.radious_, comp);
}

__TEMPLATE__
template<typename POINTTYPE> 
inline bool __HYPERBALL__::CrossesBoundaries(POINTTYPE point, 
    int32 dimension, 
		Precision_t range, 
    ComputationsCounter<diagnostic> &comp) {
	comp.UpdateDistances();
  Precision_t point_center_distance = Metric_t::Distance(center_, point, dimension);
  comp.UpdateComparisons();
	if (point_center_distance < range) {
	  return true;
	}
	comp.UpdateComparisons();
 	if (sqrt(point_center_distance) >  radious_+ sqrt(range)) {
		return false;
	} else {
	  return true;
	}
	
}

__TEMPLATE__
template<typename POINTTYPE1, typename POINTTYPE2>
inline Precision_t __HYPERBALL__::Distance(POINTTYPE1 point1, 
                                           POINTTYPE2 point2, 
                                           int32 dimension) {
  return  Metric_t::Distance(point1, point2, dimension);
}

__TEMPLATE__
inline Precision_t __HYPERBALL__::Distance(HyperBall_t &hr1,
    HyperBall_t &hr2,
    int32 dimension,
    ComputationsCounter<diagnostic> &comp) {
	comp.UpdateDistances();
	Precision_t center_distances=Metric_t::Distance(hr1.center_, 
			                           hr2.center_, dimension);
	comp.UpdateComparisons();
	Precision_t dist=sqrt(center_distances)-(hr2.radious_+hr1.radious_);
	if (dist<=0) {
	  return 0;
	} else {
		return dist*dist;
	}
    
}

__TEMPLATE__
Precision_t __HYPERBALL__::Distance(HyperBall_t &hr1,
                                  HyperBall_t &hr2,
                                  Precision_t threshold_distance,
                                  int32 dimension,
                                  ComputationsCounter<diagnostic> &comp) {
	fprintf(stderr, "Not Implemented yet\n");
	assert("false");
	return 0;
}                           

__TEMPLATE__
template<typename POINTTYPE, typename NODETYPE>                   	
inline pair<Allocator_t::template Ptr<NODETYPE>, 
	   Allocator_t::template Ptr<NODETYPE> > 
		 __HYPERBALL__::ClosestChild(Allocator_t::template Ptr<NODETYPE> left, 
		                             Allocator_t::template Ptr<NODETYPE> right,
                                 POINTTYPE point,
																 int32 dimension,
																 ComputationsCounter<diagnostic> &comp) {
	comp.UpdateDistances();
  comp.UpdateDistances();	
	Precision_t left_dist = Metric_t::Distance(pivot_left_, point, dimension);
	Precision_t right_dist = Metric_t::Distance(pivot_right_, point, dimension);
	if (left_dist<right_dist) {
	  return make_pair(left, right);
	} else {
	  return make_pair(right, left);  
	}
}                            

__TEMPLATE__
string __HYPERBALL__::Print(int32 dimension) {
  char buf[8192];
  string str("center: ");
  for(int32 i=0; i<dimension; i++){
	  sprintf(buf,"%lg ", (double) center_[i]);
    str.append(buf);
	}
	str.append("\nradious: ");
	sprintf(buf,"%lg\n", (double) radious_);
  str.append(buf);
	str.append("pivot_left: ");
  for(int32 i=0; i<dimension; i++){
	  sprintf(buf,"%lg ", (double)pivot_left_[i]);
    str.append(buf);
	}
  str.append("\npivot_right: ");
  for(int32 i=0; i<dimension; i++){
	  sprintf(buf,"%lg ", (double)pivot_right_[i]);
    str.append(buf);
	}
	str.append("\n");
	return str;
}

#undef __TEMPLATE__
#undef __HYPERBALL__
#endif // HYPER_BALL_IMPL_H_
