//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated from a template.
//
//     Manual changes to this file may cause unexpected behavior in your application.
//     Manual changes to this file will be overwritten if the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace MedicalDataManagerModel
{
    using System;
    using System.Collections.Generic;
    
    public partial class MedicationPlan
    {
        public int Id { get; set; }
        public System.Guid DeviceId { get; set; }
        public bool Critical { get; set; }
        public int DosageId { get; set; }
        public string ValidFrom { get; set; }
        public string Validto { get; set; }
        public int CompartmentNbr { get; set; }
        public int TokenId { get; set; }
    
        public virtual Device Device { get; set; }
        public virtual Dosage Dosage { get; set; }
        public virtual Token Token { get; set; }
    }
}
