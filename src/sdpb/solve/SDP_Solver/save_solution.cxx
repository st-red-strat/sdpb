#include "../SDP_Solver.hxx"
#include "../../../set_stream_precision.hxx"

#include <iomanip>

void SDP_Solver::save_solution(
  const SDP_Solver_Terminate_Reason terminate_reason,
  const std::pair<std::string, Timer> &timer_pair,
  const boost::filesystem::path &out_file) const
{
  // Internally, El::Print() sync's everything to the root core and
  // outputs it from there.  So do not actually open the file on
  // anything but the root node.

  boost::filesystem::ofstream out_stream;
  if(El::mpi::Rank() == 0)
    {
      std::cout << "Saving solution to      : " << out_file << '\n';
      out_stream.open(out_file);
      set_stream_precision(out_stream);
      out_stream << "terminateReason = \"" << terminate_reason << "\";\n"
                 << "primalObjective = " << primal_objective << ";\n"
                 << "dualObjective   = " << dual_objective << ";\n"
                 << "dualityGap      = " << duality_gap << ";\n"
                 << "primalError     = " << primal_error << ";\n"
                 << "dualError       = " << dual_error << ";\n"
                 << std::setw(16) << std::left << timer_pair.first << "= "
                 << timer_pair.second.elapsed_seconds() << ";\n"
                 << "y = {";
    }
  if(!y.blocks.empty())
    {
      El::Print(y.blocks.at(0), "", out_stream);
    }

  if(El::mpi::Rank() == 0)
    {
      out_stream << "};\nx = {";
    }

  for(auto &block : x.blocks)
    {
      El::Print(block, "", out_stream);
    }

  if(El::mpi::Rank() == 0)
    {
      out_stream << "};\n";
    }
}
