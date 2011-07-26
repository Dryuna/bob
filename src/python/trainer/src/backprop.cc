/**
 * @author Andre Anjos <andre.anjos@idiap.ch>
 * @date Mon 18 Jul 2011 18:00:18 CEST
 *
 * @brief Python bindings for Backprop training
 */

#include <boost/python.hpp>
#include "trainer/MLPBackPropTrainer.h"

using namespace boost::python;
namespace io = Torch::io;
namespace mach = Torch::machine;
namespace train = Torch::trainer;

void bind_trainer_backprop() {
  class_<train::MLPBackPropTrainer>("MLPBackPropTrainer", "Sets an MLP to perform discrimination based on vanilla error back-propagation as defined in 'Pattern Recognition and Machine Learning' by C.M. Bishop, chapter 5 or else, 'Pattern Classification' by Duda, Hart and Stork, chapter 6.", init<const mach::MLP&, size_t>((arg("machine"), arg("batch_size")), "Initializes a new MLPBackPropTrainer trainer according to a given machine settings and a training batch size.\n\nGood values for batch sizes are tens of samples. BackProp is not necessarily a 'batch' training algorithm, but performs in a smoother if the batch size is larger. This may also affect the convergence.\n\n You can also change default values for the learning rate and momentum. By default we train w/o any momenta.\n\nIf you want to adjust a potential learning rate decay, you can and should do it outside the scope of this trainer, in your own way."))
    .def("reset", &train::MLPBackPropTrainer::reset, (arg("self")), "Re-initializes the whole training apparatus to start training a new machine. This will effectively reset all Delta matrices to their initial values and set the previous derivatives to zero.")
    .add_property("batchSize", &train::MLPBackPropTrainer::getBatchSize, &train::MLPBackPropTrainer::setBatchSize)
    .add_property("learningRate", &train::MLPBackPropTrainer::getLearningRate, &train::MLPBackPropTrainer::setLearningRate)
    .add_property("momentum", &train::MLPBackPropTrainer::getMomentum, &train::MLPBackPropTrainer::setMomentum)
    .add_property("trainBiases", &train::MLPBackPropTrainer::getTrainBiases, &train::MLPBackPropTrainer::setTrainBiases)
    .def("isCompatible", &train::MLPBackPropTrainer::isCompatible, (arg("self"), arg("machine")), "Checks if a given machine is compatible with my inner settings")
    .def("train", &train::MLPBackPropTrainer::train, (arg("self"), arg("machine"), arg("input"), arg("target")), "Trains the MLP to perform discrimination. The training is executed outside the machine context, but uses all the current machine layout. The given machine is updated with new weights and biases at the end of the training that is performed a single time. Iterate as much as you want to refine the training.\n\nThe machine given as input is checked for compatibility with the current initialized settings. If the two are not compatible, an exception is thrown.\n\n.. note::\n   In BackProp, training is done in batches. You should set the batch size properly at class initialization or use setBatchSize().\n\n.. note::\n   The machine is not initialized randomly at each train() call. It is your task to call random() once at the machine you want to train and then call train() as many times as you think are necessary. This design allows for a training criteria to be encoded outside the scope of this trainer and to this type to focus only on applying the training when requested to.")
    .def("train_", &train::MLPBackPropTrainer::train_, (arg("self"), arg("machine"), arg("input"), arg("target")), "This is a version of the train() method above, which does no compatibility check on the input machine.")
    ;
}