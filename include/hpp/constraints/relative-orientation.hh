//
// Copyright (c) 2014 CNRS
// Authors: Florent Lamiraux
//
//
// This file is part of hpp-constraints.
// hpp-constraints is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// hpp-constraints is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Lesser Public License for more details. You should have
// received a copy of the GNU Lesser General Public License along with
// hpp-constraints. If not, see
// <http://www.gnu.org/licenses/>.

#ifndef HPP_RELATIVE_ORIENTATION_HH
# define HPP_RELATIVE_ORIENTATION_HH

# include <hpp/core/differentiable-function.hh>
# include <hpp/constraints/config.hh>
# include <hpp/constraints/fwd.hh>

namespace hpp {
  namespace eigen {
    typedef Eigen::Matrix <double, 3, 3> matrix3_t;
    typedef Eigen::Matrix <double, 3, 1> vector3_t;
  } // namespace eigen
  namespace constraints {
    /// Constraint on the relative orientation of two robot joints
    ///
    /// The value of the function is defined as follows:
    /// \f{eqnarray*}
    /// \mathbf{r} (\mathbf{q}) &=& \log
    /// \left((R_1(\mathbf{q})^T R_2(\mathbf{q}))^T R^*\right) =
    /// \log
    /// \left(R_2(\mathbf{q})^T R_1(\mathbf{q}) R^*\right)\\
    /// \mathbf{\dot{r}} &=&
    /// J_{\log}\left(R_2(\mathbf{q})^T R_1(\mathbf{q}) R^*\right)
    /// R_2(\mathbf{q})^T \left(J_{joint\ 1}^{\omega}(\mathbf{q}) -
    /// J_{joint\ 2}^{\omega}(\mathbf{q})\right) \mathbf{\dot{q}}\	\
    /// \f}
    /// where \f$R^*\f$ is the desired value of
    /// \f$R_1(\mathbf{q})^T R_2(\mathbf{q})\f$ and
    /// \f{eqnarray*}
    /// J_{\log} (R) &=& \frac{\|\mathbf{r}\|\sin\|\mathbf{r}\|}
    ///                  {2(1-\cos\|\mathbf{r}\|)}I_3 
    ///                  -\frac{1}{2}[\mathbf{r}]_{\times} +
    ///                  \left(\frac{1}{\|\mathbf{r}\|^2} -
    ///                  \frac{\sin\|\mathbf{r}\|}{2\|\mathbf{r}\|
    ///                  (1-\cos\|\mathbf{r}\|)}\right)
    ///                  \mathbf{r}\mathbf{r}^T
   /// \f}
    class HPP_CONSTRAINTS_DLLAPI RelativeOrientation :
      public DifferentiableFunction
    {
    public:
      EIGEN_MAKE_ALIGNED_OPERATOR_NEW
      /// Return a shared pointer to a new instance
      ///
      /// \param robot the robot the constraints is applied to,
      /// \param joint1 the first joint the orientation of which is constrained
      /// \param joint2 the second joint the orientation of which is constrained
      /// \param reference desired relative orientation
      ///        \f$R_1(\mathbf{q})^T R_2(\mathbf{q})\f$ between the joints,
      /// \param ignoreZ it true rotation about z-axis of the reference frame
      ///        is not constrained
      static RelativeOrientationPtr_t create (const DevicePtr_t& robot,
					      const JointPtr_t& joint1,
					      const JointPtr_t& joint2,
					      const matrix3_t& reference,
					      bool ignoreZ = false);
      virtual ~RelativeOrientation () throw () {}
      /// Set desired relative orientation as a rotation matrix
      void reference (const matrix3_t& reference)
      {
	reference_ = reference;
      }
      /// Get desired relative orientation
      const matrix3_t& reference () const
      {
	return reference_;
      }
      /// Whether to ignore the last component of the error vector
      ///
      /// If true, \f$R\f$ will freely rotation about \f$z\f$ axis of \f$R^*\f$.
      bool ignoreZ () const
      {
	return ignoreZ_;
      }
      ///Constructor
      ///
      /// \param robot the robot the constraints is applied to,
      /// \param joint the joint the orientation of which is constrained
      /// \param reference reference orientation of the joint,
      /// \param ignoreZ it true rotation about z-axis of the reference frame
      ///        is not constrained
      RelativeOrientation (const DevicePtr_t&, const JointPtr_t& joint1,
			   const JointPtr_t& joint2,
			   const matrix3_t& reference, bool ignoreZ);
    protected:
      /// Compute value of error
      ///
      /// \param argument configuration of the robot,
      /// \retval result error vector
      virtual void impl_compute	(vectorOut_t result,
				 ConfigurationIn_t argument)
	const throw ();
      virtual void impl_jacobian (matrixOut_t jacobian,
				  ConfigurationIn_t arg) const throw ();
    private:
      void computeError (vectorOut_t result, ConfigurationIn_t argument,
			 double& theta, bool ignoreZ) const;
      DevicePtr_t robot_;
      JointPtr_t joint1_;
      JointPtr_t joint2_;
      matrix3_t reference_;
      bool ignoreZ_;
      mutable matrix3_t Rerror_;
      mutable vector_t r_;
      mutable eigen::matrix3_t Jlog_;
      mutable matrix_t jacobian_;
    }; // class RelativeOrientation
  } // namespace constraints
} // namespace hpp
#endif // HPP_RELATIVE_ORIENTATION_HH
