#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H


#include <vector>

class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) {add(object);}

        void clear() {objects.clear();}

        void add(shared_ptr<hittable> object) {
            objects.push_back(object);
        }

        //along this ray, what's the closest object it hits if any
        bool hit (const ray& r, interval ray_t, hit_record& rec) const override {

            hit_record temp_rec; // each object writes its hit info (point, normal, t) here
            bool hit_anything = false; // tracks whether any object has been hit yet
            auto closest_so_far = ray_t.max;

            //for each object in the objects list, it whether the ray hits it but only in the range of ray_tmin to ray_tmax
            //if it does, record that something was hit and shrink closest_so_far to hit's t
            for(const auto& object : objects) {
                if(object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }
            return hit_anything;
        }
};
#endif